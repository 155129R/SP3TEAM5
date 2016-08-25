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
		PATROL,
		ATTACK,
		WEAKEN,
		CAPTURED,
		MAX_ENEMY_STATE
	};

	ENEMY_TYPE Type;
	ENEMY_STATE State;

	Enemy(ENEMY_TYPE type = GHOST_1, ENEMY_STATE state = PATROL);
	~Enemy();

	AABB Hitbox;
	float rotate;

	bool captured = false;

	bool canCatch = false;

	void Update(double dt);
	void Chase(double dt, Vector3 PlayerPos);
	void scaleDown(double dt);
	int DealDamage();
	void TakeDamage(int Damage);

	int GetHP();

private:
	//Stats
	int MAX_HP;
	int HP;
	int Attack;

	//Patrol waypoints
	Vector3 waypoint[2];
	int travel_to;
};

#endif