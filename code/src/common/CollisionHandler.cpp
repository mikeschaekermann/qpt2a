#include "CollisionHandler.h"

using namespace std;
using namespace ci;

CollisionHandler::CollisionHandler() : initialized(false) {}

CollisionHandler & CollisionHandler::initialize(Area const & screen)
{
	setScreenDimension(screen);
	clear();
	initialized = true;

	return *this;
}

CollisionHandler & CollisionHandler::clear()
{
	circleBackup.clear();
	buckets.clear();
	area.bucketMeasures.x = (float) abs(area.dimensions.x2 - area.dimensions.x1);
	area.bucketMeasures.y = (float) abs(area.dimensions.y2 - area.dimensions.y1);
	area.bucketCount.x = 1;
	area.bucketCount.y = 1;
	area.largestRadius = 0;
	initialized = false;
	backupInsert = false;

	return *this;
}

CollisionHandler & CollisionHandler::insert(Circle const & circle)
{
	Circle const * current = &circle;
	if (!backupInsert)
	{
		/// calculate grid size
		calculateBucketMeasures(*current);
	}

	

	set<unsigned int> bucketIndices = getBucketIndices(*current);
	if (bucketIndices.size() > 0)
	{
		if (!backupInsert)
		{
			current = &(circleBackup.insert(make_pair(current->index, *current)).first->second);
		}
		for (auto it = bucketIndices.begin(); it != bucketIndices.end(); ++it)
		{

			if (buckets.find(*it) == buckets.end())
			{
				buckets.insert(make_pair(*it, set<Circle const *>()));
			}
			buckets[*it].insert(current);
		}
	}

	return *this;
}

CollisionHandler & CollisionHandler::remove(unsigned int id)
{
	Circle const & cur = circleBackup[id];
	auto ids = getBucketIndices(cur);
	for (auto idIt = ids.begin(); idIt != ids.end(); ++idIt)
	{
		auto & bucketSet = buckets[*idIt];
		for (auto it = bucketSet.begin(); it != bucketSet.end(); ++it)
		{
			if ((*it)->index == cur.index)
			{
				if (bucketSet.erase(*it))
				{
					// worked
				}
				break;
			}
		}
	}

	if (circleBackup.erase(id))
	{
		// worked
	}

	return *this;
}

set<unsigned int> CollisionHandler::getCircleIndicesToCheck(Circle const & circle) const
{
	set<unsigned int> circleIndices;
	set<unsigned int> bucketIndices = getBucketIndices(circle);
	for (auto itIndices = bucketIndices.begin(); itIndices != bucketIndices.end(); ++itIndices)
	{
		auto itBucket = buckets.find(*itIndices);
		if (itBucket != buckets.end())
		{
			for (auto itCircles = itBucket->second.begin();itCircles != itBucket->second.end(); ++itCircles)
			{
				circleIndices.insert((*itCircles)->index);
			}
		}
	}

	return circleIndices;
}

unsigned int CollisionHandler::getSize() const
{
	return circleBackup.size();
}

void CollisionHandler::setScreenDimension(Area const & screen)
{
	area.dimensions = screen;
}

void CollisionHandler::calculateBucketMeasures(Circle const & circle)
{
	if (area.largestRadius < circle.radius)
	{
		float radius = area.largestRadius = circle.radius;
		float buffer = 1.f;

		float width = (float) abs(area.dimensions.x2 - area.dimensions.x1);
		float height = (float) abs(area.dimensions.y2 - area.dimensions.y1);

		area.bucketCount.x = max<int>(width / (2 * radius + buffer), 1);
		area.bucketMeasures.x = width / area.bucketCount.x;

		area.bucketCount.y = max<int>(height / (2 * radius + buffer), 1);
		area.bucketMeasures.y = height / area.bucketCount.y;

		buckets.clear();

		backupInsert = true;
		for (auto it = circleBackup.begin(); it != circleBackup.end(); ++it)
		{
			insert(it->second);
		}
		backupInsert = false;
	}
}

unsigned int CollisionHandler::getBucketIndex(ci::Vec2f const & position) const
{
	int xpos = int((position.x - area.dimensions.x1) / area.bucketMeasures.x);
	int ypos = int((position.y - area.dimensions.y1) / area.bucketMeasures.y);
	return ypos * area.bucketCount.x + xpos;
}

set<unsigned int> CollisionHandler::getBucketIndices(Circle const & circle) const
{
	set<unsigned int> indices;
	unsigned int bucketIdx = getBucketIndex(circle.position);
	float p[][2] = {{-1.f, -1.f}, { 0.f, -1.f}, { 1.f, -1.f}, {-1.f, 0.f}, { 0.f, 0.f}, { 1.f, 0.f}, {-1.f, 1.f}, { 0.f, 1.f}, { 1.f, 1.f}};
	unsigned int p_length = 9;

	/// if it intersects with other buckets add it there also
	for (unsigned int i = 0; i < p_length; ++i)
	{
		Vec2f position = circle.position + Vec2f(p[i][0] * area.bucketMeasures.x, p[i][1] * area.bucketMeasures.y);
		if (isInScreen(position))
		{
			unsigned int curBucketIdx = getBucketIndex(position);
			Area & bucket = getBucketSizeFromIndex(curBucketIdx);

			if (aabbCollision.hasCollision(bucket, circle))
			{
				indices.insert(curBucketIdx);
			}
		}
	}
	return indices;
}

Area CollisionHandler::getBucketSizeFromIndex(unsigned int idx) const
{
	Area bucketSize;
	int y = idx / area.bucketCount.x;
	int x = idx % area.bucketCount.x;

	bucketSize.x1 = (int32_t) (area.dimensions.x1 + x * area.bucketMeasures.x);
	bucketSize.y1 = (int32_t) (area.dimensions.y1 + y * area.bucketMeasures.y);
	bucketSize.x2 = (int32_t) (bucketSize.x1 + area.bucketMeasures.x);
	bucketSize.y2 = (int32_t) (bucketSize.y1 + area.bucketMeasures.y);

	return bucketSize;
}

bool CollisionHandler::isInScreen(ci::Vec2f const & position) const
{
	return position.x >= area.dimensions.x1 && 
			position.x <= area.dimensions.x2 && 
			position.y >= area.dimensions.y1 &&
			position.y <= area.dimensions.y2;
}