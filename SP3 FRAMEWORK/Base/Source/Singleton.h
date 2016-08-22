#ifndef SINGLETON_H
#define SINGLETON_H
#include "Player.h"
#include "Inventory.h"
#include <map>
#include <vector>
#include "AABBObject.h"
#include "Enemy.h"
using std::map;
using std::vector;

class Singleton
{
	public:
		static Singleton* getInstance()
		{
			static Singleton instance;
			return &instance;
		}
		
		//ADD UNIVERSAL STUFF HERE
		Player* player;

		map<AABB*, int> objectCount;

		//Environment
		std::vector<AABBObject *> Object_list;

		//Enemy
		std::vector<Enemy *> Enemy_list;
private:
	Singleton()
	{
		//init stuff here
		player = new Player();
	}
};

#endif /*SINGLETON_H*/