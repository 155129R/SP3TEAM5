#ifndef SINGLETON_H
#define SINGLETON_H
#include "Player.h"
#include "Inventory.h"
#include <map>
#include <vector>

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
private:
	Singleton()
	{
		//init stuff here
		player = new Player();
	}
};

#endif /*SINGLETON_H*/