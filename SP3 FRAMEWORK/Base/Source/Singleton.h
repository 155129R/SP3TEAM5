#ifndef SINGLETON_H
#define SINGLETON_H
#include "Inventory.h"
#include <map>
#include <vector>
#include "AABBObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Boss.h"

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
		bool gotKey;
		bool showInventory;
		bool openDoor;
		bool gotHammer;
		//ADD UNIVERSAL STUFF HERE
		Player* player;
		Boss* boss;
		Inventory* item_key;
		Inventory* item_hammer;
		Camera3* singletonCamera;

		//Environment
		vector<AABBObject *> Object_list;

		//Enemy
		vector<Enemy *> Enemy_list;

		//Inventory
		vector<Inventory*> inventory;

	Singleton()
	{
		//init stuff here
		player = new Player();
		boss = new Boss();
		item_key = new Inventory("key");
		item_hammer = new Inventory("hammer");
		mousex = 400;
		mousey = 300;


		//program_state = PROGRAM_MENU;
		//program_state = PROGRAM_GAME1;
		//program_state = PROGRAM_GAME2;
		program_state = PROGRAM_GAME3;
		//program_state = PROGRAM_GAME4;

		stateCheck = false;

		showInventory = false;

		openDoor = false;

		gotKey = false;
		gotHammer = false;
	}
	~Singleton()
	{
		delete player;
		delete boss;
		delete item_key;
	}

};

#endif /*SINGLETON_H*/