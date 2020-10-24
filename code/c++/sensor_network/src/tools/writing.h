#ifndef WRITING_H
#define WRITING_H

#include "../data_structures/data_set.h"
#include "../data_structures/solution.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

void writeDataSetToFile(string file_location, string file_name, const DataSet *data_set);

void writeSolutionToFile(string file_location, string file_name, const Solution* solution);

#endif // WRITING_H
