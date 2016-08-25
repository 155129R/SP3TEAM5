#ifndef AABBOBJECT_H
#define AABBOBJECT_H

#include "GameObject.h"
#include "AABB.h"

struct AABBObject : public GameObject
{
public:
	enum OBJECT_TYPE
	{
		//INDOOR
		BARRICADE,
		BED,
		DOOR,
		INDOORGATE,
		CHAIR,
		TOILETBOWL,
		TABLE,

		//OUTDOOR
		FOUNTAIN,
		METAL_GATE,
		HOUSE,
		HEDGE,
		BENCH,
		POT,
		HOUSE1,
		HOUSE2,
		KEY,

		//FOREST
		LOGS,
		BRIDGE,

		//GRAVEYARD
		TOMBSTONE,
		DEADTREE,

		MAX_OBJECT_TYPE,
	};

	OBJECT_TYPE Object;

	AABBObject() 
	{
		angle = 0;
		rotate.Set(0, 1, 0);
	}
	~AABBObject() {};

	AABB Hitbox;

	float angle;
	Vector3 rotate;
};

#endif
