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

    vector<int> shortest_path_to_communication_network(number_targets, -1);
    vector<int> closest_target_in_communication_network(number_targets, -1);
    for(int target_index = 0; target_index<number_targets; target_index++){
        shortest_path_to_communication_network[target_index] = data_set->getShortestPathToSource(target_index);
    }

    for(int step = 0; step<seed_vector.size(); step++){
        int target_index = seed_vector[step];

        // selecting the new sensor placement for the current target among the reception neighbors that are not used yet
        int minimum_distance_to_communication_network = number_targets+1;
        list<int>::iterator closest_to_communication_network_reception_neighbor_iterator = available_reception_arcs[target_index].end();
        for(list<int>::iterator reception_neighbor_iterator = available_reception_arcs[target_index].begin(); reception_neighbor_iterator != available_reception_arcs[target_index].end(); reception_neighbor_iterator++){
            int reception_neighbor_index = *reception_neighbor_iterator;
            if(shortest_path_to_communication_network[reception_neighbor_index] != -1 && shortest_path_to_communication_network[reception_neighbor_index] < minimum_distance_to_communication_network){
                closest_to_communication_network_reception_neighbor_iterator = reception_neighbor_iterator;
                minimum_distance_to_communication_network = shortest_path_to_communication_network[reception_neighbor_index];
            }
        }
        if(minimum_distance_to_communication_network > number_targets){
            throw "unable to find suitable reception neighbor";
        }

        // deleting the reception arc to the chosen target so that it won't be used multiple times
        int closest_to_communication_network_reception_neighbor_index = *closest_to_communication_network_reception_neighbor_iterator;
        available_reception_arcs[target_index].erase(closest_to_communication_network_reception_neighbor_iterator);

        // if a reception neighbor minimizing the distance to source has been found, the necessary sensors are added on the corresponding path to source
        int target_on_path_index = closest_to_communication_network_reception_neighbor_index;
        int closest_target_with_sensor = closest_target_in_communication_network[target_on_path_index];
        while(target_on_path_index != closest_target_with_sensor){
            if(target_on_path_index == -1){
                throw "source reached while placing missing sensors on path";
            }
            if(!solution->getTargetHasSensor(target_on_path_index)){
                solution->setTargetHasSensor(target_on_path_index, true);

                // updating distances to communication network
                for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
                    if(data_set->getShortestPath(other_target_index, target_on_path_index)<shortest_path_to_communication_network[other_target_index]){
                        shortest_path_to_communication_network[other_target_index] = data_set->getShortestPath(other_target_index, target_on_path_index);
                        closest_target_in_communication_network[other_target_index] = target_on_path_index;
                    }
                }
            }
            if(closest_target_with_sensor == -1){
                // in the particular case where the source is considered to be the closest target with sensor
                // the next target on path is the next target on path to source
                target_on_path_index = data_set->getNextTargetOnShortestPathToSource(target_on_path_index);
            } else{
                target_on_path_index = data_set->getNextTargetOnShortestPath(closest_target_with_sensor, target_on_path_index);
            }

        }

    }

    return solution;
}

Solution* simpleHeuristic(const DataSet* data_set, const vector<int>& seed_vector){
    Solution* solution = new Solution(data_set);

    int number_targets = data_set->getNumberTargets();
    int reception_level = data_set->getReceptionLevel();

    if(!(seed_vector.size() == number_targets)){
        throw "invalid argument in simpleHeuristic";
    }

    int number_targets_with_enough_reception = 0;
    if(reception_level<1){
        return solution;
    }
    vector<int> recepting_sensor_count(number_targets, 0);
    vector<bool> no_reception_potential(number_targets, false);

    auto compare = [&seed_vector, &no_reception_potential](int target_index_1, int target_index_2) {
        if(no_reception_potential[target_index_1] == no_reception_potential[target_index_2]){
            return seed_vector[target_index_1] < seed_vector[target_index_2];
        }
        return no_reception_potential[target_index_1] > no_reception_potential[target_index_2]; };
    std::priority_queue<int, std::vector<int>, decltype(compare)> adjacent_targets_without_sensors(compare);

    vector<bool> target_is_in_queue(number_targets, false);

    // initializing the priority queue with the communication neighbors of the source
    for(list<int>::const_iterator source_communication_neighbor_iterator = data_set->getSourceCommunicationNeighbors().begin(); source_communication_neighbor_iterator != data_set->getSourceCommunicationNeighbors().end(); source_communication_neighbor_iterator++){
        int source_communication_neighbor_index = *source_communication_neighbor_iterator;
        adjacent_targets_without_sensors.push(source_communication_neighbor_index);
        target_is_in_queue[source_communication_neighbor_index] = true;
    }

    while(number_targets_with_enough_reception<number_targets){
        if(adjacent_targets_without_sensors.empty()){
            throw "invalid data_set in simpleHeuristic";
        }

        int target_index = adjacent_targets_without_sensors.top();
        adjacent_targets_without_sensors.pop();

        if(!no_reception_potential[target_index]){
            // checking the reception potential of the target
            // if all the reception neighbors of the target already have enough reception, it has no reception potential
            no_reception_potential[target_index] = true;
            for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
                int reception_neighbor_index = *reception_neighbor_iterator;
                if(recepting_sensor_count[reception_neighbor_index]<reception_level){
                    no_reception_potential[target_index] = false;
                    break;
                }
            }

            // if the target has no reception potential, it is pushed back into the priority queue
            // the fact that the target has no reception potential is taken into account by the comparison
            // and the target should only reach the top of the queue when only such targets are left
            if(no_reception_potential[target_index]){
                adjacent_targets_without_sensors.push(target_index);
                continue;
            }
        }

        solution->setTargetHasSensor(target_index, true);

        for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
            int reception_neighbor_index = *reception_neighbor_iterator;

            recepting_sensor_count[reception_neighbor_index]++;
            if(recepting_sensor_count[reception_neighbor_index]==reception_level){
                number_targets_with_enough_reception++;
            }

        }

        for(list<int>::const_iterator communication_neighbor_iterator = data_set->getCommunicationNeighbors(target_index).begin(); communication_neighbor_iterator != data_set->getCommunicationNeighbors(target_index).end(); communication_neighbor_iterator++){
            int communication_neighbor_index = *communication_neighbor_iterator;

            if(!solution->getTargetHasSensor(communication_neighbor_index) && !target_is_in_queue[communication_neighbor_index]){
                adjacent_targets_without_sensors.push(communication_neighbor_index);
                target_is_in_queue[communication_neighbor_index] = true;
            }

        }
    }

    return solution;
}

Solution* hybridHeuristic(const DataSet* data_set, const vector<int>& order_vector){
    Solution* solution = new Solution(data_set);

    int number_targets = data_set->getNumberTargets();
    int reception_level = data_set->getReceptionLevel();

    if(!(order_vector.size() == number_targets)){
        throw "invalid argument in hybridHeuristic";
    }

    int number_targets_with_enpugh_reception = 0;
    if(reception_level<1){
        return solution;
    }
    vector<int> recepting_sensor_count(number_targets, 0);

    vector<int> shortest_path_to_communication_network(number_targets, -1);
    vector<int> closest_target_in_communication_network(number_targets, -1);
    for(int target_index = 0; target_index<number_targets; target_index++){
        shortest_path_to_communication_network[target_index] = data_set->getShortestPathToSource(target_index);
    }

    int step = 0;
    while(number_targets_with_enpugh_reception<number_targets && step<number_targets){
        int target_index = order_vector[step];
        step++;

        if(solution->getTargetHasSensor(target_index)){
            continue;
        }

        // checking that the current target has reception potential
        bool no_reception_potential = true;
        for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
            int reception_neighbor_index = *reception_neighbor_iterator;
            if(recepting_sensor_count[reception_neighbor_index]<reception_level){
                no_reception_potential = false;
                break;
            }
        }

        // if the target has no reception potential, no sensor should be placed on it and the iteration ends
        if(no_reception_potential){
            continue;
        }


        // placing sensors on targets belonging to the closest path between current target and current communication network.
        int target_on_path_index = target_index;
        int closest_target_with_sensor = closest_target_in_communication_network[target_on_path_index];
        while(target_on_path_index != closest_target_with_sensor){
            if(target_on_path_index == -1){
                throw "source reached while placing missing sensors on path";
            }

            // putting sensor on the target on path
            solution->setTargetHasSensor(target_on_path_index, true);

            // updating distances to communication network
            for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
                if(data_set->getShortestPath(other_target_index, target_on_path_index)<shortest_path_to_communication_network[other_target_index]){
                    shortest_path_to_communication_network[other_target_index] = data_set->getShortestPath(other_target_index, target_on_path_index);
                    closest_target_in_communication_network[other_target_index] = target_on_path_index;
                }
            }

            // updating recepting sensors count
            for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_on_path_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_on_path_index).end(); reception_neighbor_iterator++){
                int reception_neighbor_index = *reception_neighbor_iterator;
                recepting_sensor_count[reception_neighbor_index]++;
                if(recepting_sensor_count[reception_neighbor_index] == reception_level){
                    number_targets_with_enpugh_reception++;
                }
            }

            if(closest_target_with_sensor == -1){
                // in the particular case where the source is considered to be the closest target with sensor
                // the next target on path is the next target on path to source
                target_on_path_index = data_set->getNextTargetOnShortestPathToSource(target_on_path_index);
            } else{
                target_on_path_index = data_set->getNextTargetOnShortestPath(closest_target_with_sensor, target_on_path_index);
            }

        }

    }

    return solution;
}

void repareConnection(Solution* solution){
    const DataSet* data_set = solution->getDataSet();
    int number_targets = data_set->getNumberTargets();

    vector<int> shortest_path_to_communication_network(number_targets, -1);
    vector<int> closest_target_in_communication_network(number_targets, -1);
    for(int target_index = 0; target_index<number_targets; target_index++){
        shortest_path_to_communication_network[target_index] = data_set->getShortestPathToSource(target_index);
    }

    list<int> targets_to_connect;
    for(int target_index = 0; target_index<number_targets; target_index++){
        if(solution->getTargetHasSensor(target_index)){
            targets_to_connect.push_back(target_index);
        }
    }

    while(targets_to_connect.size()>0){
        // finding clostest unconnected target to the communication network
        list<int>::iterator closest_target_to_communication_network_iterator = targets_to_connect.begin();
        int minimum_distance_to_communication_network = shortest_path_to_communication_network[*closest_target_to_communication_network_iterator];
        for(list<int>::iterator target_to_connect_iterator = targets_to_connect.begin(); target_to_connect_iterator != targets_to_connect.end();target_to_connect_iterator++){
            int target_to_connect_index = *target_to_connect_iterator;
            if(shortest_path_to_communication_network[target_to_connect_index]<minimum_distance_to_communication_network){
                minimum_distance_to_communication_network = shortest_path_to_communication_network[target_to_connect_index];
                closest_target_to_communication_network_iterator = target_to_connect_iterator;
            }
        }

        int closest_target_to_communication_network_index = *closest_target_to_communication_network_iterator;

        // removing current target from targets to connect
        targets_to_connect.erase(closest_target_to_communication_network_iterator);

        // identifying the closest target in the current communication network (it might be the source)
        int closest_target_in_communication_network_index = closest_target_in_communication_network[closest_target_to_communication_network_index];

        // placing sensors on shortest path to communication network
        // a sensor is placed again on the current target even though it already has one
        int target_on_path_index = closest_target_to_communication_network_index;
        while(target_on_path_index!=closest_target_in_communication_network_index){
            // placing sensor
            solution->setTargetHasSensor(target_on_path_index, true);

            // updating the values of shortest path to communication network for the targets left to connect
            // note that the values of shortest paths to communication network may become invalid for other targets
            // but those other values will no longer be used
            for(list<int>::iterator target_to_connect_iterator = targets_to_connect.begin(); target_to_connect_iterator != targets_to_connect.end();target_to_connect_iterator++){
                int target_to_connect_index = *target_to_connect_iterator;
                if(shortest_path_to_communication_network[target_to_connect_index]>data_set->getShortestPath(target_to_connect_index, target_on_path_index)){
                    shortest_path_to_communication_network[target_to_connect_index] = data_set->getShortestPath(target_to_connect_index, target_on_path_index);
                    closest_target_in_communication_network[target_to_connect_index] = target_on_path_index;
                }
            }

            if(closest_target_in_communication_network_index == -1){
                // in the particular case where the source is considered to be the closest target in the communication network
                // the next target on path is the next target on path to source
                target_on_path_index = data_set->getNextTargetOnShortestPathToSource(target_on_path_index);
            } else{
                target_on_path_index = data_set->getNextTargetOnShortestPath(closest_target_in_communication_network_index, target_on_path_index);
            }
        }

    }

}
