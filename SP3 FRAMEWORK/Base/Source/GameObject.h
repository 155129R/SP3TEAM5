#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector3.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;

	Vector3 pos;
	Vector3 dir;
	Vector3 scale;

	bool active;
	float speed;
};

#endif