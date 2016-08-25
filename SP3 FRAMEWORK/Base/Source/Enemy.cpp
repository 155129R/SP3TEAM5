#include "Singleton.h"

Enemy::Enemy(ENEMY_TYPE type, ENEMY_STATE state) : Type(type), State(state)
{
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
			MAX_HP = 3;
			speed = 150;
			Attack = 10;
			break;
		}
	}
	active = true;
	HP = MAX_HP;
	pos.Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
	scale.Set(50, 50, 50);

	waypoint[0] = pos;
	waypoint[1].Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
	travel_to = 1;

	rotate = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::Update(double dt)
{
	//AABB hit box will always be on the Enemy
	Hitbox.UpdateAABB(this->pos);

	if (HP <= 0)
	{
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
				Vector3 dir = (waypoint[travel_to] - pos).Normalized();
				pos.x += dir.x * 80 * dt;
				pos.z += dir.z * 80 * dt;
			}

			float distance_to_player = (Singleton::getInstance()->player->getPosition() - pos).Length();
			if (distance_to_player <= 250)
			{
				State = ENEMY_STATE::ATTACK;
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

void Enemy::SetWaypoint()
{
	waypoint[0] = pos;
	waypoint[1] = pos + (Math::RandFloatMinMax(-250, 250), 0, Math::RandFloatMinMax(-250, 250));
	travel_to = 1;
}

void Enemy::Chase(double dt, Vector3 playerPos)
{
	//Move in to attack player
	Vector3 dir = (Singleton::getInstance()->player->getPosition() - pos).Normalized();
	pos += dir * speed * dt;
	if (Hitbox.Collide(Singleton::getInstance()->player->getPosition()))
	{
		//DEAL FEAR
		Singleton::getInstance()->player->InflictFear(Attack);
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