#include "metaheuristics.h"


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


Solution* localSearchMetaheuristic(int number_iterations, const Solution* initial_solution) {
    vector<int> order_vector = solutionToOrderVector(initial_solution);
    const DataSet* data_set = initial_solution->getDataSet();
    Solution* best_solution = simpleHeuristic(data_set, order_vector);

    for (int iteration = 0; iteration < number_iterations; iteration++) {
        pair<int, int> switched_target_indices = randomTwoOptSwitch(order_vector);

        Solution* test_solution = simpleHeuristic(data_set, order_vector);

        int score_difference = test_solution->getScore() - best_solution->getScore();

        // keep the change only if the score is better
        if (score_difference <= 0) {
            delete best_solution;
            best_solution = test_solution;
            if (score_difference < 0) {
                cout << "New best score: " << best_solution->getScore() << " iteration: " << iteration << endl;
            }
        } else {
            // switch back
            twoOptSwitch(order_vector, switched_target_indices.first, switched_target_indices.second);
            delete test_solution;
        }
    }

    return best_solution;
}

Solution* _simulatedAnnealingMetaheuristic(int number_iterations, const Solution* initial_solution,
                                             float T_0, float T_min, float phi) {
    float T = T_0;

    vector<int> order_vector = solutionToOrderVector(initial_solution);

    const DataSet* data_set = initial_solution->getDataSet();

    Solution* best_solution = simpleHeuristic(data_set, order_vector);

    while (T >= T_min) {
        float sum_keep = 0;
        float sum_bad_keep = 0;
        for(int iteration=0; iteration<number_iterations; iteration++){
            pair<int, int> switched_target_indices = randomTwoOptSwitch(order_vector);

            Solution* test_solution = simpleHeuristic(data_set, order_vector);

            int score_difference = test_solution->getScore() - best_solution->getScore();
            float keep_probability = exp(-score_difference / T);

            float random_value = rand() / (float)RAND_MAX;
            if (random_value <= keep_probability){
                delete best_solution;
                best_solution = test_solution;
                sum_keep += 1;
                if (keep_probability < 1) {
                    sum_bad_keep += 1;
                }
            } else {
                // switch back
                twoOptSwitch(order_vector, switched_target_indices.first, switched_target_indices.second);
                delete test_solution;
            }
        }
        cout << "T = " << T << " score: " << best_solution->getScore() << " " << exp(-1 / T) << " " << sum_keep << " " << sum_bad_keep << endl;
        T *= phi;
    }

    return best_solution;
}

Solution* simulatedAnnealingMetaheuristic(int number_iterations, const Solution* initial_solution, int number_local_search_iterations,
                                          float initial_keep_probability, float final_keep_probability, float phi) {

    cout << "Figuring out initial and final temperatures..." << endl;

    vector<int> order_vector = solutionToOrderVector(initial_solution);
    const DataSet* data_set = initial_solution->getDataSet();

    Solution* better_solution = simpleHeuristic(data_set, order_vector); // to understand
    int initial_score = better_solution->getScore();

    cout << "Initial score: " << initial_score << endl;

    const int n = 10000; // hardcoded... -> parameter ?

    float count = 0;
    float mean_score_difference = 0;
    while (count <= n) {
        // random switch
        pair<int, int> switched_target_indices = randomTwoOptSwitch(order_vector);

        Solution* test_solution = simpleHeuristic(data_set, order_vector);

        // check if test_solution is worse
        int score_difference = test_solution->getScore() - initial_score;

        // cout << count << " " << score_difference  << " " << switched_target_indices.first << " " << switched_target_indices.second << endl;
        if (score_difference > 0) {
            mean_score_difference += score_difference;
            count++;
        }

        // switch back
        twoOptSwitch(order_vector, switched_target_indices.first, switched_target_indices.second);
    }
    mean_score_difference /= n;

    float T_0 = - mean_score_difference / log(initial_keep_probability);
    float T_min = - 1 / log(final_keep_probability);

    cout << "Mean difference: " << mean_score_difference << endl;

    cout << "Initial temperature: " << T_0 << endl;
    cout << "Final temperature: " << T_min << endl;

    cout << "Number of temperature steps: " << log(T_min / T_0) / log(phi) << endl;
    cout << "Total number of iterations: " << log(T_min / T_0) / log(phi) * number_iterations << endl;

    cout << "Starting the simulated annealing..." << endl;
    Solution* best_solution = _simulatedAnnealingMetaheuristic(number_iterations, initial_solution, T_0, T_min, phi);

    cout << "Simulated annealing done. Current score: " << best_solution->getScore() << endl;

    cout << "Starting local search refining..." << endl;

    best_solution = localSearchMetaheuristic(number_local_search_iterations, best_solution);

    cout << "Final score: " << best_solution->getScore() << endl;

    return best_solution;
}

Solution* simulatedAnnealingMetaheuristicForInterface(int number_iterations, const Solution* initial_solution,
                                                      float initial_keep_probability, float final_keep_probability, float phi,
                                                      function<bool ()> continue_function, function<void (int, float)> callback_function){

    const DataSet* data_set = initial_solution->getDataSet();
    //int number_targets = data_set->getNumberTargets();

    // computing initial and final temperatures
    float T_0 = -1/log(initial_keep_probability);
    float T_min = -1/log(final_keep_probability);

    float T = T_0;

    vector<int> order_vector = solutionToOrderVector(initial_solution);

    // to improve with copy constructor
    Solution* best_solution = simpleHeuristic(data_set, order_vector);

    callback_function(best_solution->getScore(), T);

    while (T >= T_min && continue_function()) {
        float sum_keep = 0;
        float sum_bad_keep = 0;
        for(int iteration=0; iteration<number_iterations; iteration++){
            pair<int, int> switched_target_indices = randomTwoOptSwitch(order_vector);

            Solution* test_solution = simpleHeuristic(data_set, order_vector);

            int score_difference = test_solution->getScore() - best_solution->getScore();
            float keep_probability = exp(-score_difference / T);

            float random_value = rand() / (float)RAND_MAX;
            if (random_value <= keep_probability){
                delete best_solution;
                best_solution = test_solution;
                sum_keep += 1;
                if (keep_probability < 1) {
                    sum_bad_keep += 1;
                }
            } else {
                // switch back
                twoOptSwitch(order_vector, switched_target_indices.first, switched_target_indices.second);
                delete test_solution;
            }

            if(!continue_function()){
                break;
            }
        }
        callback_function(best_solution->getScore(), T);
        T *= phi;
    }

    return best_solution;
}
