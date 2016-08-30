/******************************************************************************/
/*!
\file	Boss.h
\author Chuan Guang Zhe
\par	email: 152104G@mymail.nyp.edu.sg
\brief
Class to define Boss
*/
/******************************************************************************/
#ifndef BOSS_H
#define BOSS_H

#include "GameObject.h"
#include "AABB.h"
#include "Sound.h"
using std::vector;

/******************************************************************************/
/*!
Class Boss:
\brief
Defines the Boss and its methods
*/
/******************************************************************************/
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
	void UpdateBullet(double dt);
	void Shoot(double dt, Vector3 playerpos);
	int getHP();
	void damage(int damage);
	int getAttack();
	void despawn(double dt);
	void reset();

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