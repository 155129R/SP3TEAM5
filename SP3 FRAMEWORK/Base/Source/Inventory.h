#pragma once
#include "AABB.h"
#include "Singleton.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	static void addObject(AABB* object);

private:

};

/*

#include <vector>

#include "WeaponList.h"
#include "ProjectileList.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	std::vector<Weapon*> weapons;
	std::vector<CProjectile*> bullets;

private:

};
*/