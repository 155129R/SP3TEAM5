#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector3.h"

enum GameObject
{
	P_WATER,
	P_TOTAL,
};

class ParticleObject
{
public:
	ParticleObject(PARTICLEOBJECT_TYPE = P_WATER);
	~ParticleObject();

	//Type of particle
	PARTICLEOBJECT_TYPE type;	//Type of particle
	Vector3 pos;				//Position of particle
	Vector3 vel;				//Velocity of particle
	Vector3 scale;				//Scale of particle
	float rotation;				//Rotation of particle
	float rotateSpeed;			//Rotation speed of particle

	bool active;				//Activate particle upon use
};

#endif