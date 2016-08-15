#include "Particle.h"

ParticleObject::ParticleObject(PARTICLEOBJECT_TYPE typeValue) : type(P_WATER), pos(0, 0, 0), vel(0, 0, 0), scale(0, 0, 0), rotation(0), rotateSpeed(0), active(false)
{
}

ParticleObject::~ParticleObject()
{
}