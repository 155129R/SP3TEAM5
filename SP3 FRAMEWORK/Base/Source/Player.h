#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Camera3.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	virtual void Init();
	virtual void Update(double dt);
	void GetCamera(Camera3 camera);

	void UpdateFear(double dt);
	void UpdateStamina(double dt);

	void DepleteStamina(double dt);
	void RegainStamina(double dt);
	
	float GetStamina();
	int GetFear();

	void InflictFear(int amount);

private:
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

	Camera3 camera;

	//Stamina
	float stamina;
	float recharge;

	//Weapon* equipped;
	//Inventory
};

#endif