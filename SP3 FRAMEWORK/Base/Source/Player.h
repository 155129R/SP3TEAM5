#ifndef PLAYER_H
#define PLAYER_H

#include "Camera3.h"

class Player 
{
public:
	Player();
	~Player();

	virtual void Init();
	virtual void Update(double dt);

	void UpdateMovement(double dt, unsigned short key);
	void UpdateFear(double dt);
	void UpdateStamina(double dt);

	void DepleteStamina(double dt);
	void RegainStamina(double dt);
	
	float GetStamina();
	int GetFear();

	void InflictFear(int amount);

	//weapon
	void UpdateWeapon(double dt);
	int GetWeaponType();

	Vector3 getPosition()
	{
		return position;
	}

	void setPosition(Vector3 pos)
	{
		position = pos;
	}

	bool getRun()
	{
		return Run;
	}

	void setRun(bool run)
	{
		Run = run;
	}

private:
	//weapon
	enum WEAPON_TYPE
	{
		W_RIFLE,
		W_PISTOL,
		W_VACUUM,

		W_TOTAL,
	};

	WEAPON_TYPE weaponType;

	int weapon_value = 0;

	//fear
	enum FEAR_LEVEL
	{
		NORMAL,
		CREEPY,
		PARANOID,
		SCARED,
		FAINT,

		MAX_LEVEL,
	};

	FEAR_LEVEL fear;
	int fear_value;

	//Stamina
	bool Run;
	float stamina;
	float recharge;

	//Weapon* equipped;
	//Inventory

	Vector3 position;
};

#endif