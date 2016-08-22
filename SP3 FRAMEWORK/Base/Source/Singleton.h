#ifndef SINGLETON_H
#define SINGLETON_H
#include "Player.h"

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
private:
	Singleton()
	{
		//init stuff here
		player = new Player();
	}
};

#endif /*SINGLETON_H*/