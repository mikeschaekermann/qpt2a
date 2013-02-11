#pragma once

#include <map>
#include <hash_map>
#include <array>
#include <set>
#include <unordered_set>

#include <cinder/Vector.h>
#include <cinder/Area.h>

#include "Circle.h"
#include "AABBCollisionBehavior.h"

class CollisionHandler
{
public:
	CollisionHandler();
	CollisionHandler & initialize(ci::Area const & screen);
	CollisionHandler & clear();
	CollisionHandler & insert(Circle const & circle);
	std::set<unsigned int> getCircleIndicesToCheck(Circle const & circle) const;
private:
	struct SelectionArea
	{
		ci::Area dimensions;
		ci::Vec2f bucketMeasures;
		float largestRadius;
	};

	void setScreenDimension(ci::Area const & screen);
	void calculateBucketMeasures(Circle const & circle);
	unsigned int getBucketIndex(ci::Vec2f const & position) const;
	std::set<unsigned int> getBucketIndices(Circle const & circle) const;
	ci::Area getBucketSizeFromIndex(unsigned int idx) const;
	bool isInScreen(ci::Vec2f const & position) const;

	bool initialized;
	/**
	 * when the insert is called by the calculateBucketMeasures function
	 * then some parts of the algorithm are skipped
	 */
	bool backupInsert;
	SelectionArea area;
	std::map<unsigned int, Circle const> circleBackup;
	std::map<unsigned int, std::set<Circle const *> > buckets;
	AABBCollisionBehavior aabbCollision;
};