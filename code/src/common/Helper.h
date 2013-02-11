#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>

#include "cinder/Vector.h"

using namespace std;

string stringify(ostream & in);

template<typename T>
string concatenate(string str, T value)
{
	return (str += static_cast<ostringstream*>( &(ostringstream() << value) )->str());
}

/**
 * Is true if the two hulls of the objects intersect
 */
template <class T>
bool isColliding(ci::Vec3<T> verifyingObjectPosition, float verifyingObjectRadius, ci::Vec3<T> referenceObjectPosition, float referenceObjectRadius)
{
	return (referenceObjectPosition - verifyingObjectPosition).length() < (referenceObjectRadius + verifyingObjectRadius);
}

/**
 * Is true if the veryfing object lies within the reference object
 */
template <class T>
bool isInRadiusOf(ci::Vec3<T> verifyingObjectPosition, float verifyingObjectRadius, ci::Vec3<T> referenceObjectPosition, float referenceObjectRadius)
{
	return ((referenceObjectPosition - verifyingObjectPosition).length() + (verifyingObjectRadius / 2.f)) <= referenceObjectRadius;
}