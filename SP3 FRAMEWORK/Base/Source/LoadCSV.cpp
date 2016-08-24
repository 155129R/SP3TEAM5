#include "LoadCSV.h"

void ReadFile(string file_path, int ghost[])
{
	ifstream Level(file_path);
	string Line;
	int i = 0;

	if (Level.is_open())
	{
		//Read line by line, storing it as a string
		while (getline(Level, Line))
		{
			stringstream Enemy(Line);
			string Amount;

			//Read comma by comma,storing it as the block's type
			while (getline(Enemy, Amount, ','))
			{
				ghost[i] = stoi(Amount);
				i++;
			}
		}
	}
	else
	{
		std::cout << "Error opening " << file_path << std::endl;
	}
}