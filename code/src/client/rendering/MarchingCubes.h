#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/TriMesh.h"
#include "cinder/Sphere.h"
#include "boost/thread/mutex.hpp"

using namespace std;
using namespace ci;

#define NORMAL_LENGTH 0.5773502691

class MarchingCubes
{
public:
	class Metaball
	{
	public:
		Metaball(Sphere ball);

		float getValueAt(Vec3f position, float isoLevel) const;
	private:
		Sphere ball;
		float radiusSq;
	};

	struct GridPoint
	{
		ci::Vec3f position;
		ci::Vec3f normal;
		float value;
	};

	struct GridCell
	{
		GridPoint corners[8];
	};

	struct Triangle
	{
		Vec3f vertices[3];
		Vec3f normals[3];
	};

	class Grid
	{
	public:
		Grid();
		Grid(ci::Vec3f gridSize, ci::Vec3f cubeSize, ci::Vec3f center = ci::Vec3f::zero());

		void clear();
		void applyMetaball(Metaball & metaball, float isoLevel);
		void revertMetaball(Metaball & metaball, float isoLevel);
		TriMesh triangulate(float isoLevel);
	private:
		vector<vector<vector<GridPoint> > > points;

		static int edgeTable[256];
		static int triangleTable[256][16];

		static list<Triangle> triangulate(GridCell cell, float isoLevel);
		static Vec3f interpolateCorners(GridPoint & corner1, GridPoint & corner2, float isoLevel);
		static Vec3f interpolateCornerNormals(GridPoint & corner1, GridPoint & corner2, float isoLevel);
	};

	MarchingCubes();
	MarchingCubes(Vec3f gridSize, Vec3f cellSize = Vec3f(5.f, 5.f, 5.f), float isoLevel = 1.f);
	~MarchingCubes();

	MarchingCubes &	addMetaball(Metaball metaball);
	MarchingCubes &	addMetaballSphere(Sphere metaball);
	MarchingCubes &	addMetaballCenterRadius(ci::Vec3f center, float radius);
	
	MarchingCubes &	removeMetaball(Metaball metaball);
	MarchingCubes &	removeMetaballSphere(Sphere metaball);
	MarchingCubes &	removeMetaballCenterRadius(ci::Vec3f center, float radius);
	
	MarchingCubes & calculateMesh();
	TriMesh const & getMesh() const;
	bool meshExists() const;

	boost::mutex & getMeshMutex();

private:
	TriMesh mesh;

	boost::mutex meshMutex;

	ci::Vec3f gridSize;
	ci::Vec3f cellSize;
	Grid grid;

	float isoLevel;
};

