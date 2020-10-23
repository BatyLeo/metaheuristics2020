#ifndef METAHEURISTICS_H
#define METAHEURISTICS_H

#include "heuristics.h"

#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
using namespace std;

Solution* seedVectorMetaheuristic(int number_iterations, const vector<int> &initial_seed, DataSet* data_set);

Solution* priorityOrderMetaheuristic(int number_iterations, vector<int> &priority_order, DataSet* data_set);

Solution* receptionOrientedMetaheuristic(int number_iterations, const Solution* initial_solution);

Solution* simulatedAnnealingMetaheuristic(int number_iterations, const Solution* solution, float T_0, float phi, float T_min);

#endif // METAHEURISTICS_H
