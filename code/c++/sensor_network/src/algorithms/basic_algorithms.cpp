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

void twoOptSwitch(vector<int> &order_vector, int target_index_1, int target_index_2) {
    int memory = order_vector[target_index_1];
    order_vector[target_index_1] = order_vector[target_index_2];
    order_vector[target_index_2] = memory;
    return;
}

pair<int, int> randomTwoOptSwitch(vector<int> &order_vector) {
    int number_targets = order_vector.size();

    int random_target_index_1 = rand() % number_targets;
    int random_target_index_2 = rand() % number_targets;

    //switch
    twoOptSwitch(order_vector, random_target_index_1, random_target_index_2);

    return pair<int, int>(random_target_index_1, random_target_index_2);
}
