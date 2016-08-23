#include "Singleton.h"

Enemy::Enemy(ENEMY_TYPE type, ENEMY_STATE state) : Type(type), State(state)
{
	switch (Type)
	{
		case ENEMY_TYPE::GHOST_1:
		{
			MAX_HP = 6;
			speed = 100;
			Attack = 3;
			break;
		}
		case ENEMY_TYPE::GHOST_2:
		{
			MAX_HP = 10;
			speed = 200;
			Attack = 6;
			break;
		}
		case ENEMY_TYPE::GHOST_3:
		{
			MAX_HP = 3;
			speed = 300;
			Attack = 10;
			break;
		}
	}
	active = true;
	pos.Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
	scale.Set(50, 50, 50);
}

Enemy::~Enemy()
{
}

void Enemy::Update(double dt, Vector3 playerPos)
{
	//AABB hit box will always be on the Enemy
	Hitbox.UpdateAABB(this->pos);

	if (HP <= 0)
	{
		State = ENEMY_STATE::WEAKEN;
	}

	switch (State)
	{
		case ENEMY_STATE::PATROL:
		{
			//std::cout << "PATROL" << std::endl;
			//if (this->pos.x < (initPos + Vector3(50, 50, 50)).x && this->pos.z < (initPos + Vector3(50, 50, 50)).z)
			//{
			//	if (!chasing)
			//		Chase(dt, initPos + Vector3(50, 50, 50));
			//}
			break;
		}
		case ENEMY_STATE::ATTACK:
		{
			//std::cout << "ATTACK" << std::endl;
			Chase(dt, Singleton::getInstance()->player->getPosition());
			break;
		}
		case ENEMY_STATE::WEAKEN:
		{
			if (captured)
			{
				State = ENEMY_STATE::CAPTURED;
			}
			break;
		}
		case ENEMY_STATE::CAPTURED:
		{
			Vector3 dir = (Singleton::getInstance()->player->getPosition() - pos).Normalized();
			pos += dir * 100 * dt;

			if (scale.x > 0.5 && 
				scale.y > 0.5 && 
				scale.z > 0.5)
			{
				scale -= Vector3(30, 30, 30) * dt;
			}
			else
			{
				active = false;
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