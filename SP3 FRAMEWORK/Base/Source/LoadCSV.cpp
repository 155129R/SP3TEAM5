#include "LoadCSV.h"

void ReadFile(string file_path, Vector3 level[])
{
	ifstream Level(file_path);
	string Line;

	if (Level.is_open())
	{
		//Read line by line, storing it as a string
		while (getline(Level, Line))
		{
			stringstream Enemy(Line);
			string Amount;
			int i = 1;

			//Read comma by comma,storing it as the block's type
			while (getline(Enemy, Amount, ','))
			{
				level[i].Set(i, stoi(Amount), 0);
				i++;
			}
		}
	}
	else
	{
		std::cout << "Error opening " << file_path << std::endl;
	}
}