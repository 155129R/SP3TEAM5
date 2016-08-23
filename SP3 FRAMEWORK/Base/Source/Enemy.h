#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "AABB.h"

struct Enemy : public GameObject
{
	enum ENEMY_TYPE
	{
		GHOST_1,
		GHOST_2,
		GHOST_3,

		MAX_ENEMY_TYPE,
	};
	enum ENEMY_STATE
	{
		IDLE,
		ATTACK,
		RUN,
		WEAKEN,
		MAX_ENEMY_STATE
	};

	ENEMY_TYPE Type;
	ENEMY_STATE State;

	Enemy(ENEMY_TYPE type = GHOST_1, ENEMY_STATE state = IDLE);
	~Enemy();

	AABB Hitbox;

	const int MAX_HP;
	int HP;
	const int Attack;

	void Update(double dt, Vector3 PlayerPos);
	bool Chase(Vector3 PlayerPos);
	int DealDamage();
	void TakeDamage(int Damage);
	Vector3 initPos;
	bool chasing;

	bool captured;
};

#endif