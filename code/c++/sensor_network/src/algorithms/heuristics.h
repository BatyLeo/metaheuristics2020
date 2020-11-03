#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "../data_structures/solution.h"

#include <queue>

using namespace std;

Solution* shortestPathsHeuristic(const DataSet* data_set, const vector<int> &seed_vector);

Solution* simpleHeuristic(const DataSet* data_set, const vector<int> &seed_vector);

Solution* hybridHeuristic(const DataSet* data_set, const vector<int>& order_vector);


void repareConnection(Solution* solution);

#endif // HEURISTICS_H

