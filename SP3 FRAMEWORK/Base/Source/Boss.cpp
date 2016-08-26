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

	for (std::vector<AABBObject *>::iterator it = Boss_Bullet.begin(); it != Boss_Bullet.end(); ++it)
	{
		AABBObject *bullet = (AABBObject *)*it;
		if (bullet->active)
		{
			bullet->pos += bullet->dir * bullet->speed * dt;
			float distance = (bullet->pos - pos).Length();
			if (distance >= 4000 || bullet->pos.y <= Singleton::getInstance()->player->getPosition().y - 10)
			{
				bullet->active = false;
			}
		}
	}

	std::cout << HP << std::endl;

	int rng = Math::RandIntMinMax(0, 10);

	if (HP <= 0)
	{
		HP = 0;
		boss_state = BOSS_STATE::DEAD;
	}

	switch (boss_state)
	{
		case BOSS_STATE::TRACK:
		{
			if (HP <= MAX_HP * 0.5)
			{
				boss_state = BOSS_STATE::TELEPORT;
			}
			if (mode_timer >= 5)
			{
				mode_timer = 0.0f;
				if (HP <= MAX_HP * 0.75)
				{
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
				Vector3 dir = (Singleton::getInstance()->player->getPosition() - pos).Normalized();
				pos.x += dir.x * speed * dt;
				pos.z += dir.z * speed * dt;
				mode_timer += dt;
			}
			break;
		}
		case BOSS_STATE::ATTACK:
		{
			float distance_to_player = (Singleton::getInstance()->player->getPosition() - pos).Length();
			if (mode_timer >= 8)
			{
				mode_timer = 0.0f;
				boss_state = BOSS_STATE::TRACK;
			}
			else
			{
				float timer = 0.0f;
				fire_rate = 0.8f;
				Shoot(dt, Singleton::getInstance()->player->getPosition(), timer);
				mode_timer += dt;
			}
			break;
		}
		case BOSS_STATE::TURRET:
		{
			pos = origin;
			float distance_to_player = (Singleton::getInstance()->player->getPosition() - pos).Length();
			if (mode_timer >= 8)
			{
				mode_timer = 0.0f;
				boss_state = BOSS_STATE::TRACK;
			}
			else
			{
				float timer = 0.0f;
				fire_rate = 0.2f;
				Shoot(dt, Singleton::getInstance()->player->getPosition(), timer);
				mode_timer += dt;
			}
			break;
		}
		case BOSS_STATE::TELEPORT:
		{
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
			sound.playSoundEffect2D("Sound/Boss_Hurt.mp3");
			pos.y -= speed/2 * dt;
			if (pos.y <= -200)
			{
				active = false;
			}
		}
		default:
		{
			break;
		}
	}
}

void Boss::Shoot(double dt, Vector3 playerpos, float timer)
{
	if (fire_mode_timer >= fire_rate)
	{
		//temp use 2d sound first, 3d cant seems to work on boss.cpp
		sound.playSoundEffect2D("Sound/Boss_Shoot.wav");
		//sound.playSoundEffect3D("Sound/Boss_Shoot.wav",irrklang::vec3df(pos.x, pos.y, pos.z), false);
		//std::cout << "Pew" << std::endl;
		fire_mode_timer = 0;
		AABBObject* Wisp = new AABBObject();
		Wisp->Object = AABBObject::OBJECT_TYPE::WISP;
		Wisp->active = true;
		Wisp->pos = pos;
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