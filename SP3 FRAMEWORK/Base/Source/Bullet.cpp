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

	/*if (Hitbox.Collide(enemy->pos)){
		std::cout << "Hitttttttttt" << std::endl;
		deleteBullet = true;
	}
	else */if (distanceTravelled >= range){
		deleteBullet = true;
	}

}
