#include "Inventory.h"

Inventory::Inventory()
{
}
Inventory::~Inventory()
{
}
void Inventory::addObject(AABB* object)
{
	Singleton::getInstance()->objectCount[object]++;
}


/*
#include "Inventory.h"

Inventory::Inventory()
{
	weapons.push_back(new MachineGun());
	weapons.push_back(new Shotgun());
	weapons.push_back(new DetlaffArc());

	bullets.push_back(new CProjectile());
	bullets.push_back(new ShotgunShell());
}

Inventory::~Inventory()
{
}*/