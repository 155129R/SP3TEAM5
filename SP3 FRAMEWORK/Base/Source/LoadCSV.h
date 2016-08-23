#ifndef LOADCSV_H
#define LOADCSV_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::getline;
using std::stringstream;
using std::stoi;

#include "Vector3.h"

void ReadFile(string file_path, int level[]);

#endif 