#include "Player.h"
#include "Application.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	weaponType = WEAPON_TYPE::W_PISTOL;
	weapon_value = 1;
	fear = FEAR_LEVEL::NORMAL;
	fear_value = 0;

	stamina = 100.0f;
	recharge = 3.0f;

	Run = false;
	position.Set(0, 0, 0);
}

void Player::Update(double dt)
{
	UpdateStamina(dt);
	UpdateWeapon(dt);
	UpdateFear(dt);
}

void Player::UpdateMovement(double dt, unsigned short key)
{

}

void Player::UpdateFear(double dt)
{
	if (fear_value >= 100)
	{
		fear = FEAR_LEVEL::FAINT;
	}
	else if (fear_value >= 75)
	{
		fear = FEAR_LEVEL::SCARED;
	}
	else if (fear_value >= 50)
	{
		fear = FEAR_LEVEL::PARANOID;
	}
	else if (fear_value >= 25)
	{
		fear = FEAR_LEVEL::CREEPY;
	}
	else
	{
		fear = FEAR_LEVEL::NORMAL;
	}
}

void Player::UpdateStamina(double dt)
{
	if (Run && stamina > 0.0f)
	{
		recharge = 3.0f;
		DepleteStamina(dt);
	}
	else
	{
		recharge -= dt;
		if (recharge <= 0.0f)
		{
			RegainStamina(dt);
		}
	}
}

void Player::DepleteStamina(double dt)
{
	if (stamina >= 0.0f)
	{
		stamina -= 10 * dt;
	}
}

void Player::RegainStamina(double dt)
{
	if (stamina < 100.0f)
	{
		stamina += 10 * dt;
	}
}

float Player::GetStamina()
{
	return stamina;
}

int Player::GetFear()
{
	switch (fear)
	{
		case FEAR_LEVEL::NORMAL:
		{
			return 1;
		}
		case FEAR_LEVEL::CREEPY:
		{
			return 2;
		}
		case FEAR_LEVEL::PARANOID:
		{
			return 3;
		}
		case FEAR_LEVEL::SCARED:
		{
			return 4;
		}
		case FEAR_LEVEL::FAINT:
		{
			return 5;
		}
		default:
			return 0;
	}
}

float Player::GetFearValue()
{
	return fear_value;
}

void Player::InflictFear(int amount)
{
	fear_value += amount;
	if (fear_value > 100)
	{
		fear_value = 100;
	}
}

void Player::UpdateWeapon(double dt)
{
	if (weapon_value == 1)
	{
		weaponType = WEAPON_TYPE::W_PISTOL;
	}
	else if (weapon_value == 2)
	{
		weaponType = WEAPON_TYPE::W_RIFLE;
	}
	else if (weapon_value == 3)
	{
		weaponType = WEAPON_TYPE::W_VACUUM;
	}
}

int Player::GetWeaponType()
{
	switch (weaponType)
	{
		case WEAPON_TYPE::W_PISTOL:
		{
			return 1;
		}
		case WEAPON_TYPE::W_RIFLE:
		{
			return 2;
		}
		case WEAPON_TYPE::W_VACUUM:
		{
			return 3;
		}
		default:
			return 0;
	}
}

Vector3 Player::getPosition()
{
	return position;
}

void Player::setPosition(Vector3 pos)
{
	position = pos;
}

bool Player::getRun()
{
	return Run;
}

void Player::setRun(bool run)
{
	Run = run;
}

void Player::AddHealthpack(int Amount)
{
	Singleton::getInstance()->Healthpacks += Amount;
}
int Player::getHealthPack()
{
	return Singleton::getInstance()->Healthpacks;
}
void Player::UseHealthpack()
{
	if (Singleton::getInstance()->Healthpacks > 0)
	{
		Singleton::getInstance()->Healthpacks -= 1;
		fear_value -= 20;
		if (fear_value <= 0)
		{
			fear_value = 0;
		}
	}
}

void Player::Revive()
{
	fear = FEAR_LEVEL::NORMAL;
	fear_value = 0;

	stamina = 100.0f;
}