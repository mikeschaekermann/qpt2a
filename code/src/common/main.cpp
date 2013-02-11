#include <iostream>

#include "CollisionHandler.h"

using namespace std;
using namespace ci;

int main(int argc, char* argv[])
{
	CollisionHandler h;
	set<unsigned int> list = h
		.initialize(Area(Vec2f(0, 0), Vec2f(30, 30)))
		.insert(Circle(0, Vec2f(5, 5), 1))
		.insert(Circle(1, Vec2f(15, 5), 2))
		.insert(Circle(2, Vec2f(25, 5), 3))
		.insert(Circle(3, Vec2f(5, 15), 4))
		.insert(Circle(4, Vec2f(15, 15), 4))
		.insert(Circle(5, Vec2f(25, 15), 4))
		.insert(Circle(6, Vec2f(5, 25), 4))
		.insert(Circle(7, Vec2f(15, 25), 4))
		.insert(Circle(8, Vec2f(25, 25), 4))
		.getCircleIndicesToCheck(Circle(0, Vec2f(15, 15), 14));

	cout << "Print" << endl;

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		cout << (*it) << endl;
	}
	system("pause");
}