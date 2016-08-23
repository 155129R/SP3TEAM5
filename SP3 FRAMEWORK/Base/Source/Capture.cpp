#include "Capture.h"

Capture::Capture(Vector3 pos, Vector3 dir, float rng, float spd) /* : Hitbox(AABB(this->pos, this->scale))*/
{
	position = pos;
	direction = dir;
	deleteProj = false;

	range = rng;
	speed = spd;
	vSpeed = 0;

	distanceTravelled = 0.f;

	instance = Singleton::getInstance();
}

Capture::~Capture()
{
}

void Capture::Update(double dt)
{
	distanceTravelled += dt * speed;
	position += direction * dt * speed;
	vSpeed -= (0.2f / 4) * dt;

	position.y += vSpeed;

	for (auto enemy : instance->Enemy_list)
	{
		if (enemy->active)
		{
			if (enemy->Hitbox.Collide(position)){
				enemy->captured = true;
				enemy->scale -= 500 * (float)dt;
				deleteProj = true;
			}

		}
		else if (distanceTravelled >= range){
			deleteProj = true;
		}


	}
}
