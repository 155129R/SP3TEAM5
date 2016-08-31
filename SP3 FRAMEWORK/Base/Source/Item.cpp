#include "Item.h"

Item::Item()
{
	name = "";
	howMuchItWorth = 0;
}
Item::Item(string s, int i)
{
	name = s;
	howMuchItWorth = i;
}
Item::~Item()
{
}
