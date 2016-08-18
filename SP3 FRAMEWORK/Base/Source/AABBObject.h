#ifndef AABBOBJECT_H
#define AABBOBJECT_H

#include "GameObject.h"
#include "AABB.h"

struct AABBObject : public GameObject
{
public:
	enum OBJECT_TYPE
	{
		LOGS,
		BRIDGE,

		MAX_OBJECT_TYPE,
	};

	OBJECT_TYPE Object;

	AABBObject() {};
	~AABBObject() {};

	AABB Hitbox;
};

#endif
