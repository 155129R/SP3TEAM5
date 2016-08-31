#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Item
{
public:
	string name;
	int howMuchItWorth;
	Item();
	Item(string s, int i);
	~Item();
	

private:

};
#endif
