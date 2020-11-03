#include "scripts.h"

void my_main() {
    vector<pair<float, float> > coordinates = parseCoordinates("/Users/taleboy/Desktop/metaheuristics2020/instances/captANOR1500_18_100.txt");
    float reception_radius = 1;
    float communication_radius = 2;
    int reception_level = 1;

    DataSet* data_set = new DataSet(communication_radius, reception_radius, reception_level, coordinates);
    int number_targets = data_set->getNumberTargets();

    vector<int> seed_vector(number_targets * reception_level, 0);
    for (int target_index=0; target_index<number_targets; target_index++) {
        for(int reception=0; reception<reception_level; reception++){
            seed_vector[target_index*reception_level + reception] = target_index;
        }
    }
    shuffle(seed_vector.begin(), seed_vector.end(), std::default_random_engine(0));

    Solution* heuristic_solution = shortestPathsHeuristic(data_set, seed_vector);

    // Solution* best_solution = _simulatedAnnealingMetaheuristic(2000, heuristic_solution, 100, 0.01, 0.9);
    Solution* best_solution = simulatedAnnealingMetaheuristic(10000, heuristic_solution, 1000000, 0.80, 1e-8, 0.95);

    writeSolutionToFile(".", "sol.txt", best_solution);

    delete data_set;
    delete heuristic_solution;
    delete best_solution;
}

void my_main_2() {
    const string instances_folder = "/Users/taleboy/Desktop/metaheuristics2020/instances/";
    const string solutions_folder = "/Users/taleboy/Desktop/metaheuristics2020/solutions";

    const vector<pair<int, int> > radius_vector{
        pair<int, int>(1, 1), pair<int, int>(1, 2), pair<int, int>(2, 2), pair<int, int>(2, 3)
    };
    const vector<string> instance_file_names{ "captANOR150_7_4.txt", "captANOR225_8_10.txt", "captANOR625_12_100.txt",
                                              "captANOR900_15_20.txt", "captANOR1500_15_100.txt", "captANOR1500_18_100.txt" };
    for (vector<string>::const_iterator file_name_iterator = instance_file_names.begin(); file_name_iterator != instance_file_names.end(); ++file_name_iterator) {
        string instance_path = instances_folder + *file_name_iterator;
        vector<pair<float, float> > coordinates = parseCoordinates(instance_path);
        for (int reception_level = 1; reception_level <= 3; reception_level++) {
            for (vector<pair<int, int> >::const_iterator radius_iterator = radius_vector.begin(); radius_iterator != radius_vector.end(); ++radius_iterator) {
                float reception_radius = radius_iterator->first;
                float communication_radius = radius_iterator->second;

                cout << "Instance: " << *file_name_iterator << endl;
                cout << "Reception level: " << reception_level << endl;
                cout << "Reception radius: " << reception_radius << endl;
                cout << "Communication radius: " << communication_radius << endl;

                DataSet* data_set = new DataSet(communication_radius, reception_radius, reception_level, coordinates);
                int number_targets = data_set->getNumberTargets();

                vector<int> seed_vector(number_targets * reception_level, 0);
                for (int target_index=0; target_index<number_targets; target_index++) {
                    for(int reception=0; reception<reception_level; reception++){
                        seed_vector[target_index*reception_level + reception] = target_index;
                    }
                }
                shuffle(seed_vector.begin(), seed_vector.end(), std::default_random_engine(0));

                Solution* heuristic_solution = shortestPathsHeuristic(data_set, seed_vector);

                Solution* best_solution = simulatedAnnealingMetaheuristic(10000, heuristic_solution, 1000000, 0.80, 1e-8, 0.95);

                string solution_folder = solutions_folder + "/k_" + to_string(reception_level) + "/" + to_string((int)reception_radius) + "_" + to_string((int)communication_radius);
                writeSolutionToFile(solution_folder, *file_name_iterator, best_solution);

                delete data_set;
                delete heuristic_solution;
                delete best_solution;
            }
        }
    }
    return;
}

void my_main_3() {
    vector<int> order_vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    vector<int> reverse_target_indices = randomKOptSwitch(order_vector, 9);

    for (int i = 0; i < order_vector.size(); i++) {
        cout << order_vector[i] << endl;
    }

    cout << "\n";

    kOptSwitch(order_vector, reverse_target_indices);
    for (int i = 0; i < order_vector.size(); i++) {
        cout << order_vector[i] << endl;
    }
    return;
}

void my_main_4() {
    string instance = "captANOR1500_18_100";
    vector<pair<float, float> > coordinates = parseCoordinates("/Users/taleboy/Desktop/metaheuristics2020/instances/" + instance + ".txt");
    float reception_radius = 1;
    float communication_radius = 1;
    int reception_level = 1;

    DataSet* data_set = new DataSet(communication_radius, reception_radius, reception_level, coordinates);
    int number_targets = data_set->getNumberTargets();
    string solutions_folder = "/Users/taleboy/Desktop/metaheuristics2020/solutions/";
    string solution_path = solutions_folder + "/k_" + to_string(reception_level) + "/" + to_string((int)reception_radius) + "_" + to_string((int)communication_radius) + "/" + instance + ".txt";
    vector<bool> sensor_placement = parseSensorPlacement(solution_path, number_targets);

    Solution* initial_solution = new Solution(data_set, sensor_placement);

    Solution* best_solution = multipleNeighborhoodsSearch(5e5, initial_solution, 4);

    cout << "Done !" << endl;

    cout << "Best solution admissible ? " << best_solution->checkAdmissible() << endl;

    delete initial_solution;
    delete best_solution;
}

void my_main_5() {
    string instance = "captANOR1500_18_100";
    vector<pair<float, float> > coordinates = parseCoordinates("/Users/taleboy/Desktop/metaheuristics2020/instances/" + instance + ".txt");
    float reception_radius = 1;
    float communication_radius = 1;
    int reception_level = 1;

    DataSet* data_set = new DataSet(communication_radius, reception_radius, reception_level, coordinates);
    int number_targets = data_set->getNumberTargets();
    string solutions_folder = "/Users/taleboy/Desktop/metaheuristics2020/solutions/";
    string solution_path = solutions_folder + "/k_" + to_string(reception_level) + "/" + to_string((int)reception_radius) + "_" + to_string((int)communication_radius) + "/" + instance + ".txt";
    vector<bool> sensor_placement = parseSensorPlacement(solution_path, number_targets);

    Solution* initial_solution = new Solution(data_set, sensor_placement);

    Solution* best_solution = localSearchMetaheuristic(1e6, initial_solution); // multipleNeighborhoodsSearch(1e5, initial_solution, 5);

    cout << "Done !" << endl;

    cout << "Best solution admissible ? " << best_solution->checkAdmissible() << endl;

    delete initial_solution;
    delete best_solution;
}

void my_main_6() {
    const string instances_folder = "/Users/taleboy/Desktop/metaheuristics2020/instances/";
    const string solutions_folder = "/Users/taleboy/Desktop/metaheuristics2020/solutions";
    const string new_solutions_folder = "/Users/taleboy/Desktop/metaheuristics2020/better_solutions";

    const vector<pair<int, int> > radius_vector{
        pair<int, int>(1, 1), pair<int, int>(1, 2), pair<int, int>(2, 2), pair<int, int>(2, 3)
    };
    const vector<string> instance_file_names{ "captANOR150_7_4.txt", "captANOR225_8_10.txt", "captANOR625_12_100.txt",
                                              "captANOR900_15_20.txt", "captANOR1500_15_100.txt", "captANOR1500_18_100.txt" };
    for (vector<string>::const_iterator file_name_iterator = instance_file_names.begin(); file_name_iterator != instance_file_names.end(); ++file_name_iterator) {
        string instance_path = instances_folder + *file_name_iterator;
        vector<pair<float, float> > coordinates = parseCoordinates(instance_path);
        for (int reception_level = 1; reception_level <= 3; reception_level++) {
            for (vector<pair<int, int> >::const_iterator radius_iterator = radius_vector.begin(); radius_iterator != radius_vector.end(); ++radius_iterator) {
                float reception_radius = radius_iterator->first;
                float communication_radius = radius_iterator->second;

                cout << "Instance: " << *file_name_iterator << endl;
                cout << "Reception level: " << reception_level << endl;
                cout << "Reception radius: " << reception_radius << endl;
                cout << "Communication radius: " << communication_radius << endl;

                DataSet* data_set = new DataSet(communication_radius, reception_radius, reception_level, coordinates);
                int number_targets = data_set->getNumberTargets();

                string solution_path = solutions_folder + "/k_" + to_string(reception_level) + "/" + to_string((int)reception_radius) + "_" + to_string((int)communication_radius) + "/" + *file_name_iterator;
                vector<bool> sensor_placement = parseSensorPlacement(solution_path, number_targets);

                Solution* initial_solution = new Solution(data_set, sensor_placement);

                Solution* best_solution = multipleNeighborhoodsSearch(5e5, initial_solution, 4);

                cout << "Done !\n" << endl;

                //cout << "Best solution admissible ? " << best_solution->checkAdmissible() << endl;

                string new_solution_path = new_solutions_folder + "/k_" + to_string(reception_level) + "/" + to_string((int)reception_radius) + "_" + to_string((int)communication_radius);
                writeSolutionToFile(new_solution_path, *file_name_iterator, best_solution);

                delete initial_solution;
                delete best_solution;
                delete data_set;
            }
        }
    }
    return;
}
