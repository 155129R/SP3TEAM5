#include "Bullet.h"

vector<Bullet*> Bullet::bulletList;

Bullet::Bullet(Vector3 pos, Vector3 dir, float rng, float spd, int dmg)
{
	position = pos;
	direction = dir;
	deleteBullet = false;

	range = rng;
	speed = spd;
	damage = dmg;
	vSpeed = 0;

	distanceTravelled = 0.f;
}

Bullet::~Bullet()
{
}

void Bullet::Update(double dt)
{
	distanceTravelled += dt * speed;
	position += direction * dt * speed;
	vSpeed -= (0.2f / 4) * dt;

	position.y += vSpeed;

	if (distanceTravelled >= range){
		deleteBullet = true;
	}
}