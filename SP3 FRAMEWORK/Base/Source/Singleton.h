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
		bool showShop;
		bool openDoor;
		bool gotHammer;

		bool gameWin;
		bool gameCredit;

		double maxPistolAmmo;
		double pistolMag;
		double pistolAmmo;

		double maxRifleAmmo;
		double rifleMag;
		double rifleAmmo;

		bool gotClear;

		float footstepDelay;
		int money;
		bool noClip;

		int Healthpacks;

		//ADD UNIVERSAL STUFF HERE
		Player* player;
		Boss* boss;
		Item* item_key;
		Item* item_hammer;
		Item* item_ghost1;
		Item* item_ghost2;
		Item* item_ghost3;
		Item* item_torchlight;
		Item* item_nightVision;
		Item* item_radar;
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
        player->Init();

		boss = new Boss();
		item_key = new Item("key", 0);
		item_hammer = new Item("hammer", 0);
		item_ghost1 = new Item("ghost1", 10);
		item_ghost2 = new Item("ghost2", 20);
		item_ghost3 = new Item("ghost3", 30);
		item_torchlight = new Item("torchlight", 0);
		item_nightVision = new Item("nightvision", 0);
		item_radar = new Item("radar", 0);
		mousex = 400;
		mousey = 300;
		footstepDelay = 0;
		if (inventory2ndRow.size() < 6)
		{
			inventory2ndRow.push_back(item_torchlight);
			inventory2ndRow.push_back(item_nightVision);
			inventory2ndRow.push_back(item_radar);
		}		

		program_state = PROGRAM_MENU;
		//program_state = PROGRAM_GAME1;
		//program_state = PROGRAM_GAME2;
		//program_state = PROGRAM_GAME3;
		//program_state = PROGRAM_GAME4;
		//program_state = PROGRAM_HUB;

		stateCheck = false;

		showInventory = false;
		showShop = false;
		openDoor = false;

		gotKey = false;
		gotHammer = false;
		gotClear = false;
		noClip = false;

		gameWin = false;
		gameCredit = false;

		money = 0;
		Healthpacks = 0;

		maxPistolAmmo = 20;
		pistolMag = 2;
		pistolAmmo = maxPistolAmmo;

		maxRifleAmmo = 10;
		rifleMag = 1;
		rifleAmmo = maxRifleAmmo;
	}
	~Singleton()
	{
		delete boss;
		delete item_key;
		delete item_hammer;
		delete item_ghost1;
		delete item_ghost2;
		delete item_ghost3;
		delete item_torchlight;
		delete item_nightVision;
		delete item_radar;
	}
};

#endif /*SINGLETON_H*/