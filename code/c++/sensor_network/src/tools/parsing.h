#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string fileNameToString(string file_name);

vector<pair<float, float> > parseCoordinates(string file_name);

vector<bool> parseSensorPlacement(string file_name, int number_targets);

#endif // PARSING_H
