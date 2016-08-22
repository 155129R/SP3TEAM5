#ifndef AABB_H
#define AABB_H

#include "Vector3.h"

struct AABB
{
	AABB(Vector3 pos = Vector3(0, 0, 0), Vector3 size = Vector3(0, 0, 0));
	~AABB();

	void UpdateAABB(Vector3 pos);
	void Resize(Vector3 size);
	bool Collide(Vector3 pos, bool is3D = false);

	Vector3 max, min;
	Vector3 pos, size;
};

#endif