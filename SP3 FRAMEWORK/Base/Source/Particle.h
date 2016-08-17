#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

enum PARTICLEOBJECT_TYPE
{	
	P_WATER,
	P_FOUNTAIN_WATER1,
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