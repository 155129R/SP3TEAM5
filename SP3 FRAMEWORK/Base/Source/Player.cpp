#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	fear = FEAR_LEVEL::NORMAL;
	fear_value = 0;
	stamina = 500.0f;
	recharge = 5.0f;
}

void Player::GetCamera(Camera3 camera)
{
	this->camera = camera;
}

void Player::Update(double dt)
{
	pos = camera.position;

	UpdateStamina(dt);

	UpdateFear(dt);
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
	if (camera.Run && stamina > 0.0f)
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