#include "Singleton.h"

Enemy::Enemy(ENEMY_TYPE type, ENEMY_STATE state) : Type(type), State(state)
{
	sound.Init();

	canCatch = false;
	switch (Type)
	{
		case ENEMY_TYPE::GHOST_1:
		{
			MAX_HP = 6;
			speed = 50;
			Attack = 3;
			break;
		}
		case ENEMY_TYPE::GHOST_2:
		{
			MAX_HP = 10;
			speed = 100;
			Attack = 6;
			break;
		}
		case ENEMY_TYPE::GHOST_3:
		{
			MAX_HP = 1;
			speed = 150;
			Attack = 10;
			break;
		}
	}
	active = true;
	HP = MAX_HP;
	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME1)
	{
		pos.Set(Math::RandFloatMinMax(-800, 800), 0, Math::RandFloatMinMax(-800, 800));
		waypoint[1].Set(Math::RandFloatMinMax(-1000, 1000), 0, Math::RandFloatMinMax(-1000, 1000));
	}
	else if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME2)
	{
		pos.Set(Math::RandFloatMinMax(-800, 800), 0, Math::RandFloatMinMax(-800, 800));
		waypoint[1].Set(Math::RandFloatMinMax(-1000, 1000), 0, Math::RandFloatMinMax(-1000, 1000));
	}
	else
	{
		pos.Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
		waypoint[1].Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
	}
	scale.Set(50, 50, 50);
	dir.SetZero();

	waypoint[0] = pos;
	travel_to = 1;

	rotate = 0.0f;
	cooldown = 0.8f;
}

Enemy::~Enemy()
{
}

void Enemy::Update(double dt)
{
	//AABB hit box will always be on the Enemy
	Hitbox.UpdateAABB(this->pos);

	sound.Update(irrklang::vec3df(Singleton::getInstance()->singletonCamera->position.x, Singleton::getInstance()->singletonCamera->position.y, Singleton::getInstance()->singletonCamera->position.z),
		irrklang::vec3df(-Singleton::getInstance()->singletonCamera->view.x, Singleton::getInstance()->singletonCamera->view.y, -Singleton::getInstance()->singletonCamera->view.z));

	if (HP <= 0)
	{
		HP = 0;
		State = ENEMY_STATE::WEAKEN;
	}
	if (canCatch)
	{
		State = ENEMY_STATE::CAPTURED;
	}

	switch (State)
	{
		case ENEMY_STATE::PATROL:
		{
			float distance = (waypoint[travel_to] - pos).Length();
			if (distance <= 200)
			{
				switch (travel_to)
				{
					case 0:
					{
						travel_to = 1;
						break;
					}
					case 1:
					{
						travel_to = 0;
						break;
					}
				}
			}
			else
			{	
				dir = (waypoint[travel_to] - pos).Normalized();
				pos.x += dir.x * 50 * dt;
				pos.z += dir.z * 50 * dt;
			}

			float distance_to_player = (Singleton::getInstance()->player->getPosition() - pos).Length();
			if (distance_to_player <= 250)
			{
				State = ENEMY_STATE::ATTACK;

				switch (Type)
				{
				case ENEMY_TYPE::GHOST_1:
				{
					sound.playSoundEffect2D("Sound/ghost1.mp3");
					//sound.playSoundEffect3D("Sound/ghost1.mp3", irrklang::vec3df(pos.x, pos.y, pos.z), false);
					break;
				}
				case ENEMY_TYPE::GHOST_2:
				{
					sound.playSoundEffect2D("Sound/ghost2.mp3");
					//sound.playSoundEffect3D("Sound/ghost2.mp3", irrklang::vec3df(pos.x, pos.y, pos.z), false);
					break;
				}
				case ENEMY_TYPE::GHOST_3:
				{
					sound.playSoundEffect2D("Sound/ghost3.mp3");
					//sound.playSoundEffect3D("Sound/ghost3.mp3", irrklang::vec3df(pos.x, pos.y, pos.z), false);
					break;
				}
				}
			}

			break;
		}
		case ENEMY_STATE::ATTACK:
		{
			float distance_to_player = (Singleton::getInstance()->player->getPosition() - pos).Length();
			if (distance_to_player >= 300)
			{
				State = ENEMY_STATE::PATROL;
			}
			else
			{
				Chase(dt, Singleton::getInstance()->player->getPosition());
			}
			break;
		}
		case ENEMY_STATE::WEAKEN:
		{	
			break;
		}
		case ENEMY_STATE::CAPTURED:
		{
			Vector3 dir = (Singleton::getInstance()->player->getPosition() - pos).Normalized();
			Vector3 vaccum = dir + Singleton::getInstance()->singletonCamera->right;
			pos += vaccum + Vector3(0, -1, 0) * 80 * dt;

			if (scale.x > 0.5 && 
				scale.y > 0.5 && 
				scale.z > 0.5)
			{
			scale -= Vector3(50, 50, 50) * dt;
			rotate += 200 * dt;
			}
			else
			{
				switch (Type)
				{
					case ENEMY_TYPE::GHOST_1:
					{
						if (Singleton::getInstance()->inventory.size() < 6)
							Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost1);
						break;
					}
					case ENEMY_TYPE::GHOST_2:
					{
						if (Singleton::getInstance()->inventory.size() < 6)
							Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost2);
						break;
					}
					case ENEMY_TYPE::GHOST_3:
					{
						if (Singleton::getInstance()->inventory.size() < 6)
							Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost3);
						break;
					}
				}
				
				active = false;
				canCatch = false;
				break;
			}
		}

		default:
		{
			break;
		}
	}
}

void Enemy::Chase(double dt, Vector3 playerPos)
{
	//Move in to attack player
	Vector3 dir = (playerPos - pos).Normalized();
	pos += dir * speed * dt;
	if (Hitbox.Collide(playerPos))
	{
		//DEAL FEAR
		if (cooldown >= 0.8f)
		{
			cooldown = 0.0f;
			Singleton::getInstance()->player->InflictFear(Attack);
		}
		else
		{
			cooldown += dt;
		}
		State = ENEMY_STATE::PATROL;
	}
}

int Enemy::DealDamage()
{
	return Attack;
}

void Enemy::TakeDamage(int Damage)
{	
	HP -= Damage;
}

int Enemy::GetHP()
{
	return HP;
}

int Enemy::GetMaxHP()
{
	return MAX_HP;
}