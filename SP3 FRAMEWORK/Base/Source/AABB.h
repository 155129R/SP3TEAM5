#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include <map>

using std::map;

struct AABB
{
	AABB(Vector3 pos = Vector3(0, 0, 0), Vector3 size = Vector3(0, 0, 0));
	~AABB();

	void UpdateAABB(Vector3 pos);
	void Resize(Vector3 size);
	bool Collide(Vector3 pos);

	Vector3 max, min;
	Vector3 pos, size;

	map<AABB*, int> objectMap;
};

#endif