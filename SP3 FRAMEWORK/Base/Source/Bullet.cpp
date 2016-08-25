#include "Bullet.h"

Bullet::Bullet(Vector3 pos, Vector3 dir, float rng, float spd, int dmg) /* : Hitbox(AABB(this->pos, this->scale))*/
{
	position = pos;
	direction = dir;
	deleteBullet = false;

	range = rng;
	speed = spd;
	damage = dmg;
	vSpeed = 0;

	distanceTravelled = 0.f;

	instance = Singleton::getInstance();
}

Bullet::~Bullet()
{
}

void Bullet::Update(double dt)
{
	
	//Hitbox.UpdateAABB(this->position);

	distanceTravelled += dt * speed;
	position += direction * dt * speed;
	vSpeed -= (0.2f / 4) * dt;

	position.y += vSpeed;

	for (auto enemy : instance->Enemy_list)
	{
		if (enemy->active)
		{
			if (enemy->GetHP() >= 0)
			{
				if (enemy->Hitbox.Collide(position)){
					enemy->TakeDamage(1);
					deleteBullet = true;
				}
				else if (distanceTravelled >= range){
					deleteBullet = true;
				}
			}
		}
	}
	if (Singleton::getInstance()->boss->active)
	{
		if (Singleton::getInstance()->boss->Hitbox.Collide(position))
		{
			Singleton::getInstance()->boss->damage(1);
			deleteBullet = true;
		}
		else if (distanceTravelled >= range)
		{
			deleteBullet = true;
		}
	}
}
