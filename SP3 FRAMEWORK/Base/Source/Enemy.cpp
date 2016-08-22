#include "Enemy.h"

Enemy::Enemy(ENEMY_TYPE type, ENEMY_STATE) : MAX_HP(10), Attack(5), Hitbox(AABB(this->pos, this->scale)), initPos(this->pos), chasing(false)
{
	HP = MAX_HP;
}

Enemy::~Enemy()
{
}

void Enemy::Update(double dt, Vector3 PlayerPos)
{
	//AABB hit box will always be on the Enemy
	Hitbox.UpdateAABB(this->pos);

	if (HP > 0)
	{
		switch (State)
		{
			case ENEMY_STATE::IDLE:
			{
				if (this->pos.x < (initPos + Vector3(50, 50, 50)).x && this->pos.z < (initPos + Vector3(50, 50, 50)).z)
				{
					if (!chasing)
					Chase(initPos + Vector3(50, 50, 50));
				}
				break;
			}
			case ENEMY_STATE::ATTACK:
			{
				if (Chase(PlayerPos))
				{
					std::cout << "Deal DMG" << std::endl;
					break;
				}
				break;
			}
			case ENEMY_STATE::RUN:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

bool Enemy::Chase(Vector3 PlayerPos)
{
	float distance = (pos - PlayerPos ).LengthSquared();
	float threshhold = 50;

	if (distance < threshhold)
	{
		if (pos.x > PlayerPos.x)
		{
			pos.x -= speed;
		}
		if (pos.x < PlayerPos.x)
		{
			pos.x += speed;
		}
		if (pos.z < PlayerPos.z)
		{
			pos.z += speed;
		}
		if (pos.z > PlayerPos.z)
		{
			pos.z -= speed;
		}
		chasing = true;
		return true;
	}
	return false;
}

int Enemy::DealDamage()
{
	return Attack;
}

void Enemy::TakeDamage(int Damage)
{
	if (State != ENEMY_STATE::WEAKEN)
	{
		HP -= Damage;
	}
	if (HP <= 0)
	{
		State = ENEMY_STATE::WEAKEN;
	}
}