#ifndef SINGLETON_H
#define SINGLETON_H
#include "Item.h"
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
			PROGRAM_HUB,
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
		Item* item_key;
		Item* item_hammer;
		Item* item_ghost1;
		Item* item_ghost2;
		Item* item_ghost3;
		Camera3* singletonCamera;

		//Environment
		vector<AABBObject *> Object_list;

		//Enemy
		vector<Enemy *> Enemy_list;

		//Item
		vector<Item*> inventory;
		vector<Item*> inventory2ndRow;

	Singleton()
	{
		//init stuff here
		player = new Player();
		boss = new Boss();
		item_key = new Item("key");
		item_hammer = new Item("hammer");
		item_ghost1 = new Item("ghost1");
		item_ghost2 = new Item("ghost2");
		item_ghost3 = new Item("ghost3");
		mousex = 400;
		mousey = 300;


		program_state = PROGRAM_MENU;
		//program_state = PROGRAM_GAME1;
		//program_state = PROGRAM_GAME2;
		//program_state = PROGRAM_GAME3;
		//program_state = PROGRAM_GAME4;
		//program_state = PROGRAM_HUB;

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