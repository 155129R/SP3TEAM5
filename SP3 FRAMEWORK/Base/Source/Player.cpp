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
	fireRate = 5;

	fear = FEAR_LEVEL::NORMAL;
	fear_value = 0;

	stamina = 500.0f;
	recharge = 5.0f;

	Run = false;
	position.Set(0, 0, 0);
}

void Player::Update(double dt)
{

//	pos = camera.position;
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
		recharge = 5.0f;
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