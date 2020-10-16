#include "heuristics.h"

Solution* shortestPathsHeuristic(const DataSet* data_set, const vector<int>& seed_vector){
    Solution* solution = new Solution(data_set);

    int number_targets = data_set->getNumberTargets();
    int reception_level = data_set->getReceptionLevel();

    if(!(seed_vector.size() == number_targets*reception_level)){
        throw "invalid argument in shortestPathsHeuristic";
    }

    // copying reception arcs in order to keep track of those that are already used
    vector<list<int> > available_reception_arcs = vector<list<int> >(number_targets);
    for(int target_index = 0; target_index<number_targets; target_index++){
        available_reception_arcs[target_index] = list<int>(data_set->getReceptionNeighbors(target_index));
    }

    for(int step = 0; step<seed_vector.size(); step++){
        int target_index = seed_vector[step];

        // initializing vectors that will be used to find the shortest path between the current target and the source
        // the cost of the path is the number of targets on the path that have no sensor
        vector<int> distance_to_source = vector<int>(number_targets, -1);
        vector<int> next_target_in_shortest_path_to_source = vector<int>(number_targets, -1);

        auto compare = [&distance_to_source](int target_index_1, int target_index_2) { return distance_to_source[target_index_1] < distance_to_source[target_index_2]; };
        std::priority_queue<int, std::vector<int>, decltype(compare)> targets_to_explore(compare);

        // initializing the priority queue with the communication neighbors of the source
        for(list<int>::const_iterator source_communication_neighbor_iterator = data_set->getSourceCommunicationNeighbors().begin(); source_communication_neighbor_iterator != data_set->getSourceCommunicationNeighbors().end(); source_communication_neighbor_iterator++){
            int source_communication_neighbor_index = *source_communication_neighbor_iterator;
            // if the target has a sensor, connection to the source costs 0, otherwise it costs 1
            distance_to_source[source_communication_neighbor_index] = solution->getTargetHasSensor(source_communication_neighbor_index) ? 0 : 1;
            targets_to_explore.push(source_communication_neighbor_index);
            // the value -1 denotes that the next target on the shortest path is the source
            next_target_in_shortest_path_to_source[source_communication_neighbor_index] = -1;
        }

        // completing the computation of the distances to source
        while(!targets_to_explore.empty()){
            int explored_target_index = targets_to_explore.top();
            targets_to_explore.pop();
            for(list<int>::const_iterator communication_neighbor_iterator = data_set->getCommunicationNeighbors(explored_target_index).begin(); communication_neighbor_iterator != data_set->getCommunicationNeighbors(explored_target_index).end(); communication_neighbor_iterator++){
                int communication_neighbor_index = *communication_neighbor_iterator;
                // if the communication neighbor has not been explored yet (distance -1), its distance is set and it is added to the targets to explore
                if(distance_to_source[communication_neighbor_index] == -1){
                    int arc_cost = solution->getTargetHasSensor(communication_neighbor_index) ? 0 : 1;
                    distance_to_source[communication_neighbor_index] = distance_to_source[explored_target_index] + arc_cost;
                    targets_to_explore.push(communication_neighbor_index);
                    next_target_in_shortest_path_to_source[communication_neighbor_index] = explored_target_index;
                }
            }
        }

        // selecting the new sensor placement for the current target among the reception neighbors that are not used yet
        int minimum_distance_to_source = number_targets+1;
        list<int>::iterator closest_to_source_reception_neighbor_iterator = available_reception_arcs[target_index].begin();
        for(list<int>::iterator reception_neighbor_iterator = available_reception_arcs[target_index].begin(); reception_neighbor_iterator != available_reception_arcs[target_index].end(); reception_neighbor_iterator++){
            int reception_neighbor_index = *reception_neighbor_iterator;
            if(distance_to_source[reception_neighbor_index] != -1 && distance_to_source[reception_neighbor_index] < minimum_distance_to_source){
                closest_to_source_reception_neighbor_iterator = reception_neighbor_iterator;
                minimum_distance_to_source = distance_to_source[reception_neighbor_index];
            }
        }
        if(minimum_distance_to_source > number_targets){
            throw "unable to find suitable reception neighbor";
        }

        // deleting the reception arc to the chosen target so that it won't be used multiple times
        int closest_to_source_reception_neighbor_index = *closest_to_source_reception_neighbor_iterator;
        available_reception_arcs[target_index].erase(closest_to_source_reception_neighbor_iterator);

        // if a reception neighbor minimizing the distance to source has been found, the necessary sensors are added on the corresponding path to source
        int number_sensors_to_add = minimum_distance_to_source;
        int target_on_path_index = closest_to_source_reception_neighbor_index;
        while(number_sensors_to_add > 0){
            if(target_on_path_index == -1){
                throw "source reached while placing missing sensors on path";
            }
            if(!solution->getTargetHasSensor(target_on_path_index)){
                solution->setTargetHasSensor(target_on_path_index, true);
                number_sensors_to_add--;
            }
            target_on_path_index = next_target_in_shortest_path_to_source[target_on_path_index];
        }

    }

    return solution;
}
