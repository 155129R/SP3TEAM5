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
		
		enum PROGRAM_STATE
		{
			PROGRAM_MENU,
			PROGRAM_GAME1,
			PROGRAM_GAME2,
			PROGRAM_GAME3,
			PROGRAM_GAME4,
			PROGRAM_EXIT,

			PROGRAM_TOTAL
		};

		PROGRAM_STATE program_state;
		bool stateCheck;

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

		program_state = PROGRAM_MENU;
		stateCheck = false;
	}
};

#endif /*SINGLETON_H*/