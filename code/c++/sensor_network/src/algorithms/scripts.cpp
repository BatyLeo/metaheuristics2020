#include "scripts.h"

void my_main() {
    vector<pair<float, float> > coordinates = parseCoordinates("/Users/taleboy/Desktop/metaheuristics2020/instances/captANOR1500_18_100.txt");
    float captation_radius = 1;
    float reception_radius = 1;
    int reception_level = 3;

    DataSet* data_set = new DataSet(reception_radius, captation_radius, reception_level, coordinates);
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
