#ifndef _VEC3_H_
#define _VEC3_H_

#include "Proc.h"

class Vec3{
public:

	struct{	float x, y, z; };

	float G3D(Vec3 const &other)
	{
	    Vec3 Distance;
		Distance.x = other.x - this->x;
		Distance.y = other.y - this->y;
		Distance.z = other.z - this->z;
		return sqrt((Distance.x * Distance.x) + (Distance.y * Distance.y) + (Distance.z * Distance.z));
	}
};

#endif