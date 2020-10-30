#ifndef METAHEURISTICS_H
#define METAHEURISTICS_H

#include "heuristics.h"
#include "basic_algorithms.h"

#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <functional>
using namespace std;

Solution* seedVectorMetaheuristic(int number_iterations, const vector<int> &initial_seed, DataSet* data_set);

Solution* localSearchMetaheuristic(int number_iterations, const Solution* initial_solution);

Solution* _simulatedAnnealingMetaheuristic(int number_iterations, const Solution* initial_solution, float T_0, float T_min,  float phi);

Solution* simulatedAnnealingMetaheuristic(int number_iterations, const Solution* initial_solution, int number_local_search_iterations,
                                          float initial_keep_probability, float final_keep_probability, float phi);

Solution* simulatedAnnealingMetaheuristicForInterface(int number_iterations, const Solution* initial_solution,
                                                      float initial_keep_probability, float final_keep_probability, float phi,
                                                      function<bool ()> continue_function, function<void (int, float)> callback_function);

Solution* multipleNeighborhoodsSearch(int number_loops, const Solution* initial_solution, int k_max);

#endif // METAHEURISTICS_H
