#include "Singleton.h"

Boss::Boss()
{
	boss_state = BOSS_STATE::TRACK;

	MAX_HP = 200;
	speed = 300;
	Attack = 0;
	fire_rate = 0.0f;

	mode_timer = 0.0f;
	fire_mode_timer = 0.0f;

	active = true;
	HP = MAX_HP;
	pos.Set(0, 500, 0);
	scale.Set(80, 80, 80);

	origin = pos;
}

Boss::~Boss()
{
}

void Boss::Update(double dt)
{
	//AABB hit box will always be on the boss
	Hitbox.UpdateAABB(this->pos - Vector3(0, 100, 0));

	//Updates
	UpdateBullet(dt);

	//Randomness to determine attacks
	int rng = Math::RandIntMinMax(0, 10);

	//Sets the boss state to dead when hp reaches 0
	if (HP <= 0)
	{
		HP = 0;
		boss_state = BOSS_STATE::DEAD;
	}

	//Boss state machine
	switch (boss_state)
	{
		//Tracking state, boss will track the player for 5 seconds
		case BOSS_STATE::TRACK:
		{
			//Teleport instead if HP <= 50%
			if (HP <= MAX_HP * 0.5)
			{
				boss_state = BOSS_STATE::TELEPORT;
			}
			if (mode_timer >= 5)
			{
				mode_timer = 0.0f;

				//Decides between 2 different type of attack if HP <= 75%
				if (HP <= MAX_HP * 0.75)
				{
					//20% chance to use turret mode
					if (rng <= 2)
					{
						boss_state = BOSS_STATE::TURRET;
					}
					else
					{
						boss_state = BOSS_STATE::ATTACK;
					}
				}
				else
				{
					boss_state = BOSS_STATE::ATTACK;
				}
			}
			else
			{
				//Locates the player and move towards the player
				Vector3 dir = (Singleton::getInstance()->player->getPosition() - pos).Normalized();
				pos.x += dir.x * speed * dt;
				pos.z += dir.z * speed * dt;
				mode_timer += dt;
				if ((Singleton::getInstance()->player->getPosition() - pos).Length() <= 450)
				{
					mode_timer = 5.0f;
				}
			}
			break;
		}
		case BOSS_STATE::ATTACK:
		{
			//Reverts back to tracking mode after 8 seconds in attack mode
			if (mode_timer >= 8)
			{
				mode_timer = 0.0f;
				boss_state = BOSS_STATE::TRACK;
			}
			else
			{
				//Fires at the player meanwhile
				fire_rate = 0.8f;
				Shoot(dt, Singleton::getInstance()->player->getPosition());
				mode_timer += dt;
			}
			break;
		}
		case BOSS_STATE::TURRET:
		{
			//Teleports back to the origin position
			pos = origin;
			//Reverts back to tracking mode after 6 seconds in turret mode
			if (mode_timer >= 6)
			{
				mode_timer = 0.0f;
				boss_state = BOSS_STATE::TRACK;
			}
			else
			{
				//Fires at the player meanwhile
				fire_rate = 0.2f;
				Shoot(dt, Singleton::getInstance()->player->getPosition());
				mode_timer += dt;
			}
			break;
		}
		case BOSS_STATE::TELEPORT:
		{
			//Teleports to within 1000 units near the player 
			pos = Singleton::getInstance()->player->getPosition() + Vector3(Math::RandFloatMinMax(-500, 500), 0, Math::RandFloatMinMax(-500, 500)) + origin;
			if (rng <= 2)
			{
				boss_state = BOSS_STATE::TURRET;
			}
			else
			{
				boss_state = BOSS_STATE::ATTACK;
			}
			break;
		}
		case BOSS_STATE::DEAD:
		{
			//Despawns when dead
			despawn(dt);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Boss::UpdateBullet(double dt)
{
	for (std::vector<AABBObject *>::iterator it = Boss_Bullet.begin(); it != Boss_Bullet.end(); ++it)
	{
		AABBObject *bullet = (AABBObject *)*it;
		if (bullet->active)
		{
			bullet->pos += bullet->dir * bullet->speed * dt;
			//Despawns when bullet travels 4000 units or goes underground
			float distance = (bullet->pos - pos).Length();
			if (distance >= 4000 || bullet->pos.y <= Singleton::getInstance()->player->getPosition().y - 10)
			{
				bullet->active = false;
			}
			if (bullet->Hitbox.Collide(Singleton::getInstance()->player->getPosition()))
			{
				Singleton::getInstance()->player->InflictFear(20);
				bullet->active = false;
			}
		}
	}
}

void Boss::Shoot(double dt, Vector3 playerpos)
{
	//Fires bullet according to firerate
	if (fire_mode_timer >= fire_rate)
	{
		sound.playSoundEffect2D("Sound/Boss_Shoot.wav");
		fire_mode_timer = 0;
		AABBObject* Wisp = new AABBObject();
		Wisp->Object = AABBObject::OBJECT_TYPE::WISP;
		Wisp->active = true;
		Wisp->pos = pos;
		//1.5 bullet speed when HP <= 25%
		if (HP >= MAX_HP * 0.25)
		{
			Wisp->speed = 800;
		}
		else
		{
			Wisp->speed = 1200;
		}
		Wisp->scale.Set(100, 100, 100);
		Wisp->dir = (playerpos - Wisp->pos).Normalized();
		Boss_Bullet.push_back(Wisp);
	}
	else
	{
		fire_mode_timer += dt;
	}
}

int Boss::getMAX()
{
	return MAX_HP;
}

int Boss::getHP()
{
	return HP;
}

void Boss::damage(int damage)
{
	HP -= damage;
}

int Boss::getAttack()
{
	return Attack;
}

void Boss::despawn(double dt)
{
	pos.y -= speed / 2 * dt;
	if (pos.y <= -200)
	{
		active = false;
		Boss_Bullet.clear();
	}
	sound.playSoundEffect2D("Sound/Boss_Hurt.mp3");
}

void Boss::reset()
{
	boss_state = BOSS_STATE::TRACK;

	MAX_HP = 200;
	speed = 300;
	Attack = 0;
	fire_rate = 0.0f;

	mode_timer = 0.0f;
	fire_mode_timer = 0.0f;

	active = true;
	HP = MAX_HP;
	pos.Set(0, 500, 0);
	scale.Set(80, 80, 80);

	origin = pos;
}