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

void kOptSwitch(vector<int> &order_vector, const vector<int> &target_indices) {
    int k = target_indices.size();

    int memory = order_vector[target_indices[0]];
    int memory2;

    for (int i = 1; i < k; i++) {
        memory2 = order_vector[target_indices[i]];
        order_vector[target_indices[i]] = memory;
        memory = memory2;
    }
    order_vector[target_indices[0]] = memory;
    return;
}

vector<int> randomKOptSwitch(vector<int> &order_vector, int k) {
    int number_targets = order_vector.size();
    vector<int> random_target_indices(k, 0);
    vector<int> reverse_target_indices(k, 0);
    vector<bool> already_picked(number_targets, false);

    for (int i = 0; i < k; i++) {
        int random_index = rand() % number_targets;
        // reroll if target was previously picked
        while (already_picked[random_index]) {
            random_index = rand() % number_targets;
        }
        already_picked[random_index] = true;
        random_target_indices[i] = random_index;
        reverse_target_indices[k - i - 1] = random_index;
    }

    kOptSwitch(order_vector, random_target_indices);
    return reverse_target_indices;
}
