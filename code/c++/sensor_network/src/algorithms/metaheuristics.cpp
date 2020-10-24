#include "metaheuristics.h"
#include "basic_algorithms.h"


Solution* seedVectorMetaheuristic(int number_iterations, const vector<int> &initial_seed, DataSet* data_set){
    int number_targets = data_set->getNumberTargets();

    vector<int> seed = vector<int>(initial_seed);

    Solution* solution = shortestPathsHeuristic(data_set, seed);

    for(int iteration = 0; iteration<number_iterations; iteration++){
        int random_target_index_1 = rand()%number_targets;
        int random_target_index_2 = rand()%number_targets;

        int memory = seed[random_target_index_1];
        seed[random_target_index_1] = seed[random_target_index_2];
        seed[random_target_index_2] = memory;

        Solution* test_solution = shortestPathsHeuristic(data_set, seed);

        if(test_solution->getScore()<=solution->getScore()){
            delete solution;
            solution = test_solution;
        } else{
            memory = seed[random_target_index_1];
            seed[random_target_index_1] = seed[random_target_index_2];
            seed[random_target_index_2] = memory;
            delete test_solution;
        }

    }

    return solution;

}

Solution* priorityOrderMetaheuristic(int number_iterations, vector<int> &priority_order, DataSet* data_set){
    int number_targets = data_set->getNumberTargets();
    int reception_level = data_set->getReceptionLevel();

    Solution* best_solution = new Solution(data_set);
    for(int target_index = 0; target_index<number_targets; target_index++){
        best_solution->setTargetHasSensor(target_index, true);
    }

    for(int iteration = 0; iteration<number_iterations; iteration++){
        Solution* solution = new Solution(data_set);

        int number_targets_with_enough_reception = 0;
        vector<int> recieving_sensor_count(number_targets, 0);

        auto compare = [&priority_order](int target_index_1, int target_index_2) { return priority_order[target_index_1] > priority_order[target_index_2]; };
        std::priority_queue<int, std::vector<int>, decltype(compare)> adjacent_targets_without_sensors(compare);

        // initializing the priority queue with the communication neighbors of the source
        for(list<int>::const_iterator source_communication_neighbor_iterator = data_set->getSourceCommunicationNeighbors().begin(); source_communication_neighbor_iterator != data_set->getSourceCommunicationNeighbors().end(); source_communication_neighbor_iterator++){
            int source_communication_neighbor_index = *source_communication_neighbor_iterator;
            adjacent_targets_without_sensors.push(source_communication_neighbor_index);
        }

        while(number_targets_with_enough_reception<number_targets){
            if(adjacent_targets_without_sensors.empty()){
                throw "invalid data_set in priorityOrderMetaheuristic";
            }

            int target_index = adjacent_targets_without_sensors.top();
            adjacent_targets_without_sensors.pop();

            solution->setTargetHasSensor(target_index, true);

            // updating the numbers of recieving sensors for reception neighbors
            int useful_receptions_count = 0;
            for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
                int reception_neighbor_index = *reception_neighbor_iterator;

                recieving_sensor_count[reception_neighbor_index]++;
                if(recieving_sensor_count[reception_neighbor_index]==reception_level){
                    number_targets_with_enough_reception++;
                }
                if(recieving_sensor_count[reception_neighbor_index]<=reception_level){
                    useful_receptions_count++;
                }

            }

            // updating the priority order of the current target with the number of useful receptions it got
            priority_order[target_index] = (priority_order[target_index]*iteration+useful_receptions_count)/(iteration+1);

            for(list<int>::const_iterator communication_neighbor_iterator = data_set->getCommunicationNeighbors(target_index).begin(); communication_neighbor_iterator != data_set->getCommunicationNeighbors(target_index).end(); communication_neighbor_iterator++){
                int communication_neighbor_index = *communication_neighbor_iterator;

                if(!solution->getTargetHasSensor(communication_neighbor_index)){
                    adjacent_targets_without_sensors.push(communication_neighbor_index);
                }

            }

        }

        if(solution->getScore()<=best_solution->getScore()){
            delete best_solution;
            best_solution = solution;
        } else{
            delete solution;
        }
    }

    return best_solution;
}

Solution* receptionOrientedMetaheuristic(int number_iterations, const Solution* initial_solution){
    const DataSet* data_set = initial_solution->getDataSet();
    int number_targets = data_set->getNumberTargets();
    int reception_level = data_set->getReceptionLevel();

    // building best solution and structures
    Solution* best_solution = new Solution(data_set);
    vector<int> number_recepted_targets(number_targets, 0);
    vector<vector<int> > recieving_targets(number_targets, vector<int>(reception_level, -1));
    for(int target_index = 0; target_index<number_targets; target_index++){
        int number_reception_neighbors = data_set->getReceptionNeighbors(target_index).size();
        vector<int> reception_neighbors(number_reception_neighbors, 0);

        if(reception_level>number_reception_neighbors){
            throw "reception level too high in receptionOrientedMetaheuristic";
        }

        // ordering reception neighbors randomly
        int index = 0;
        for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
            reception_neighbors[index] = *reception_neighbor_iterator;
            index++;
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(reception_neighbors.begin(), reception_neighbors.end(), std::default_random_engine(seed));

        int reception = 0;
        for(int index = 0; index<number_reception_neighbors && reception<reception_level; index++){
            int reception_neighbor_index = reception_neighbors[index];
            if(initial_solution->getTargetHasSensor(reception_neighbor_index)){
                recieving_targets[target_index][reception] = reception_neighbor_index;
                number_recepted_targets[reception_neighbor_index]++;
                reception++;
            }
        }
    }
    // placing sensors on targets that recept at least one target
    for(int target_index = 0; target_index<number_targets; target_index++){
        if(number_recepted_targets[target_index]>0){
            best_solution->setTargetHasSensor(target_index, true);
        }
    }
    repareConnection(best_solution);

    // iterations
    for(int iteration = 0; iteration<number_iterations; iteration++){
        // choosing a target randomly
        int target_index = rand()%number_targets;

        // choosing recieving targets randomly among reception neighbors
        int number_reception_neighbors = data_set->getReceptionNeighbors(target_index).size();
        vector<int> reception_neighbors(number_reception_neighbors, 0);
        int index = 0;
        for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
            reception_neighbors[index] = *reception_neighbor_iterator;
            index++;
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(reception_neighbors.begin(), reception_neighbors.end(), std::default_random_engine(seed));
        //if(iteration%2 == 0){
        //    auto compare = [&number_recepted_targets](int target_index_1,int target_index_2) { return (number_recepted_targets[target_index_1]>number_recepted_targets[target_index_2]); };
        //    sort(reception_neighbors.begin(), reception_neighbors.end(), compare);
        //}

        // updatig recieving targets
        vector<int> number_recepted_targets_variation(number_targets, 0);
        vector<int> former_recieving_targets(recieving_targets[target_index]);
        for(int reception = 0; reception<reception_level; reception++){
            int former_recieving_target = recieving_targets[target_index][reception];
            number_recepted_targets_variation[former_recieving_target]--;
        }
        for(int reception = 0; reception<reception_level; reception++){
            int new_recieving_target = reception_neighbors[reception];
            number_recepted_targets_variation[new_recieving_target]++;
            recieving_targets[target_index][reception] = new_recieving_target;
        }

        bool required_sensors_changed = false;
        for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
            if(number_recepted_targets_variation[other_target_index]>0 && number_recepted_targets[other_target_index] == 0){
                required_sensors_changed = true;
            }else if(number_recepted_targets_variation[other_target_index]>0 && number_recepted_targets_variation[other_target_index]+number_recepted_targets[other_target_index] == 0){
                required_sensors_changed = true;
            }
            number_recepted_targets[other_target_index] = number_recepted_targets_variation[other_target_index]+number_recepted_targets[other_target_index];
        }

        // if the changes in recieving targets caused required sensors to change
        // a new solution is created and its score is compared to the current best solution
        if(required_sensors_changed){
            Solution* tested_solution = new Solution(data_set);

            // placing sensors on targets that recept at least one target
            for(int target_index = 0; target_index<number_targets; target_index++){
                if(number_recepted_targets[target_index]>0){
                    tested_solution->setTargetHasSensor(target_index, true);
                }
            }
            repareConnection(tested_solution);

            if(tested_solution->getScore()>best_solution->getScore()){
                delete tested_solution;

                // undoing last changes
                recieving_targets[target_index] = former_recieving_targets;
                for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
                    number_recepted_targets[other_target_index] = number_recepted_targets[other_target_index] - number_recepted_targets_variation[other_target_index];
                }

            } else{
                delete best_solution;
                best_solution = tested_solution;
            }

        }
    }

    return best_solution;
}

Solution* simulatedAnnealingMetaheuristic(int number_iterations, const Solution* solution, float T_0, float phi, float T_min){
    float T = T_0;

    vector<int> order_vector = solutionToOrderVector(solution);

    const DataSet* data_set = solution->getDataSet();
    int number_targets = data_set->getNumberTargets();

    // Solution* my_solution;

    Solution* best_solution = simpleHeuristic(data_set, order_vector);

    while (T >= T_min) {
        for(int iteration=0; iteration<number_iterations; iteration++){
            int random_target_index_1 = rand() % number_targets;
            int random_target_index_2 = rand() % number_targets;

            // switch
            int memory = order_vector[random_target_index_1];
            order_vector[random_target_index_1] = order_vector[random_target_index_2];
            order_vector[random_target_index_2] = memory;

            Solution* test_solution = simpleHeuristic(data_set, order_vector);

            int score_difference = test_solution->getScore() - best_solution->getScore();
            float keep_probability = exp(-score_difference / T);

            float random_value = rand() / (float)RAND_MAX;
            if (random_value <= keep_probability){
                delete best_solution;
                best_solution = test_solution;
            } else {
                // switch back
                memory = order_vector[random_target_index_1];
                order_vector[random_target_index_1] = order_vector[random_target_index_2];
                order_vector[random_target_index_2] = memory;
                delete test_solution;
            }

        }
        T *= phi;
    }

    return best_solution;
}

