#ifndef CAPTURE_H
#define CAPTURE_H

#include "GameObject.h"
#include "AABB.h"
#include "Vector3.h"
#include "Enemy.h"
#include "Sound.h"
#include "Singleton.h"
#include "Player.h"
#include <vector>

using std::vector;

class Capture : public GameObject
{
public:
	Capture(Vector3 pos, Vector3 dir, float rng, float spd);
	~Capture();

	void Update(double dt);

	Vector3 position;
	bool deleteProj;

	AABB Hitbox;

	//Player* player;

private:
	Vector3 direction;

	

	float range;
	float speed;
	float distanceTravelled;
	float vSpeed;

	Singleton* instance;
};

#endif