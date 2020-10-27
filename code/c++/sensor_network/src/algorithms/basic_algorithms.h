#ifndef BASIC_ALGORITHMS_H
#define BASIC_ALGORITHMS_H

#include <random>
#include <utility>

#include "../data_structures/solution.h"

using namespace std;

vector<int> solutionToOrderVector(const Solution *solution);

void twoOptSwitch(vector<int> &order_vector, int target_index_1, int target_index_2);

pair<int, int> randomTwoOptSwitch(vector<int> &order_vector);

#endif // BASIC_ALGORITHMS_H
