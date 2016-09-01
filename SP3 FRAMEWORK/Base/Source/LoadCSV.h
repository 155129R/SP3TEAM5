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

void ReadFile(string file_path, int ghost[]);
void ReadDialogue(string file_path, string dialogue[]); 

#endif 