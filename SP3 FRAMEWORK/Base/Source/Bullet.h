#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
#include "AABB.h"
#include "Vector3.h"
#include "Enemy.h"
#include "Sound.h"
#include <vector>

using std::vector;

class Bullet : public GameObject
{
public:
	Bullet(Vector3 pos, Vector3 dir, float rng, float spd, int dmg);
	~Bullet();

	void Update(double dt);

	Vector3 position;
	bool deleteBullet;

	AABB Hitbox;
	Enemy* enemy;

private:
	Vector3 direction;

	float range;
	float speed;
	float distanceTravelled;
	float vSpeed;

	int damage;
	
};

#endif