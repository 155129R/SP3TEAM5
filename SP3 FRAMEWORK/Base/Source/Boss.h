#ifndef BOSS_H
#define BOSS_H

#include "GameObject.h"
#include "AABB.h"
#include "Sound.h"
using std::vector;

class Boss : public GameObject
{
public:
	enum BOSS_STATE
	{
		TRACK,
		ATTACK,
		TURRET,
		TELEPORT,
		DEAD,

		MAX_BOSS_STATE
	};

	Boss();
	~Boss();

	BOSS_STATE boss_state;

	AABB Hitbox;

	vector<AABBObject *> Boss_Bullet;

	void Update(double dt);
	void Shoot(double dt, Vector3 playerpos , float timer);
	int getHP();
	void damage(int damage);
	int getAttack();

private:
	int MAX_HP;
	int HP;
	int Attack;
	float fire_rate;

	float mode_timer;
	float fire_mode_timer;

	Vector3 origin;

	Sound sound;
};

#endif