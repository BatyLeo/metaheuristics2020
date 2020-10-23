#include "basic_algorithms.h"

vector<int> solutionToOrderVector(const Solution* solution) {
    int number_targets = solution->getDataSet()->getNumberTargets();

    vector<int> order_vector(number_targets, 0);
    int count_from_below = 0;
    int count_from_above = number_targets;

    for (int target_index=0; target_index<number_targets; target_index++) {
        if (solution->getTargetHasSensor(target_index)) {
            order_vector[target_index] = --count_from_above;
        } else {
            order_vector[target_index] = count_from_below++;
        }
    }
    return order_vector;
}
