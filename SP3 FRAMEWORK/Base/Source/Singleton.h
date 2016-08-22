#ifndef SINGLETON_H
#define SINGLETON_H
#include "Inventory.h"
#include <map>
#include <vector>
#include "AABBObject.h"
#include "Enemy.h"
#include "Player.h"
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

		double mousex;
		double mousey;

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

		program_state = PROGRAM_GAME3;
		stateCheck = false;

		mousex = 400;
		mousey = 300;
	}
};

#endif /*SINGLETON_H*/