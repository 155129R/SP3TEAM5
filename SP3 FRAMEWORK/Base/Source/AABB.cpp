#include "AABB.h"

AABB::AABB(Vector3 pos, Vector3 size)
{
	this->pos = pos;
	this->size = size;

	max.x = pos.x + size.x * 0.5;
	max.y = pos.y + size.y * 0.5;
	max.z = pos.z + size.z * 0.5;
	min.x = pos.x - size.x * 0.5;
	min.y = pos.y - size.y * 0.5;
	min.z = pos.z - size.z * 0.5;
}

AABB::~AABB()
{
}

void AABB::UpdateAABB(Vector3 pos, Vector3 size)
{
	this->pos = pos;
	this->size = size;

	max.x = pos.x + size.x * 0.5;
	max.y = pos.y + size.y * 0.5;
	max.z = pos.z + size.z * 0.5;
	min.x = pos.x - size.x * 0.5;
	min.y = pos.y - size.y * 0.5;
	min.z = pos.z - size.z * 0.5;
}

bool AABB::Collide(Vector3 pos)
{
	if ((pos.x >= min.x) && (pos.x <= max.x) &&
		(pos.y >= min.y) && (pos.y <= max.y) &&
		(pos.z >= min.z) && (pos.z <= max.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}