#include "data_set.h"
#include "solution.h"

DataSet::DataSet(float communication_radius, float reception_radius, int reception_level, const vector<pair<float, float> > &target_coordinates) :
communication_radius(communication_radius), reception_radius(reception_radius), reception_level(reception_level) ,target_coordinates(target_coordinates)
{
    number_targets = target_coordinates.size();

    buildCommunicationArcs();
    buildReceptionArcs();
    buildShortestPaths();

    solutions = vector<Solution*>();
}


DataSet::~DataSet(){
    for(int solution_index = 0; solution_index<solutions.size(); solution_index++){
        delete solutions[solution_index];
    }
}

float DataSet::getCommunicationRadius() const{
    return communication_radius;
}

float DataSet::getReceptionRadius() const{
    return reception_radius;
}

int DataSet::getNumberTargets() const{
    return number_targets;
}

int DataSet::getReceptionLevel() const{
    return reception_level;
}

const pair<float, float>& DataSet::getTargetCoordinates(int target_index) const{
    return target_coordinates[target_index];
}

const list<int>& DataSet::getCommunicationNeighbors(int target_index) const{
    return communication_neighbors[target_index];
}

const list<int>& DataSet::getReceptionNeighbors(int target_index) const{
    return reception_neighbors[target_index];
}

const list<int>& DataSet::getSourceCommunicationNeighbors() const{
    return source_communication_neighbors;
}

int DataSet::getShortestPathToSource(int target_index) const{
    return shortest_paths_to_source[target_index];
}

int DataSet::getNextTargetOnShortestPathToSource(int target_index) const{
    return next_target_on_shortest_path_to_source[target_index];
}

int DataSet::getShortestPath(int origin_target_index, int destination_target_index) const{
    return shortest_paths[origin_target_index][destination_target_index];
}

int DataSet::getNextTargetOnShortestPath(int origin_target_index, int destination_target_index) const{
    return next_target_on_shortest_path[origin_target_index][destination_target_index];
}


void DataSet::setCommunicationRadius(float new_communication_radius){
    communication_radius = new_communication_radius;
    buildCommunicationArcs();
    buildShortestPaths();
}

void DataSet::setReceptionRadius(float new_reception_radius){
    reception_radius = new_reception_radius;
    buildReceptionArcs();
    buildShortestPaths();
}

void DataSet::setReceptionLevel(int new_reception_level){
    reception_level = new_reception_level;
}

void DataSet::addSolution(Solution* solution){
    solutions.push_back(solution);
}

void DataSet::removeSolution(int solution_index){
    delete solutions[solution_index];
    solutions.erase(solutions.begin()+solution_index);
}

int DataSet::getNumberSolutions(){
    return solutions.size();
}

Solution* DataSet::getSolution(int solution_index){
    return solutions[solution_index];
}



void DataSet::buildCommunicationArcs(){
    communication_neighbors = vector<list<int> >(number_targets, list<int>());
    source_communication_neighbors = list<int>();
    for(int target_index = 0; target_index<number_targets; target_index++){
        for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
            float delta_x = target_coordinates[target_index].first - target_coordinates[other_target_index].first;
            float delta_y = target_coordinates[target_index].second - target_coordinates[other_target_index].second;
            float square_distance = delta_x*delta_x + delta_y*delta_y;

            // adding communication arc
            if(square_distance <= communication_radius*communication_radius && target_index != other_target_index){
                communication_neighbors[target_index].push_back(other_target_index);
            }
        }

        // adding connection arc from source
        float square_distance_to_source = target_coordinates[target_index].first*target_coordinates[target_index].first + target_coordinates[target_index].second*target_coordinates[target_index].second;
        if(square_distance_to_source<=communication_radius*communication_radius){
            source_communication_neighbors.push_back(target_index);
        }
    }
}

void DataSet::buildReceptionArcs(){
    reception_neighbors = vector<list<int> >(number_targets, list<int>());
    for(int target_index = 0; target_index<number_targets; target_index++){
        for(int other_target_index = 0; other_target_index<number_targets; other_target_index++){
            float delta_x = target_coordinates[target_index].first - target_coordinates[other_target_index].first;
            float delta_y = target_coordinates[target_index].second - target_coordinates[other_target_index].second;
            float square_distance = delta_x*delta_x + delta_y*delta_y;

            // adding reception arc
            if(square_distance <= reception_radius*reception_radius){
                reception_neighbors[target_index].push_back(other_target_index);
            }
        }
    }
}

void DataSet::buildShortestPaths(){
    shortest_paths = vector<vector<int> >(number_targets, vector<int>(number_targets, -1));
    shortest_paths_to_source = vector<int>(number_targets, -1);

    next_target_on_shortest_path = vector<vector<int> >(number_targets, vector<int>(number_targets, -1));
    next_target_on_shortest_path_to_source = vector<int>(number_targets, -1);

    // computing shortest paths to source
    queue<int> targets_to_explore;

    // initializing the queue with the communication neighbors of the source
    for(list<int>::const_iterator source_communication_neighbor_iterator = source_communication_neighbors.begin(); source_communication_neighbor_iterator != source_communication_neighbors.end(); source_communication_neighbor_iterator++){
        int source_communication_neighbor_index = *source_communication_neighbor_iterator;
        // if the target has a sensor, connection to the source costs 0, otherwise it costs 1
        shortest_paths_to_source[source_communication_neighbor_index] = 1;
        targets_to_explore.push(source_communication_neighbor_index);
        // the value -1 denotes that the next target on the shortest path is the source
        next_target_on_shortest_path_to_source[source_communication_neighbor_index] = -1;
    }

    while(!targets_to_explore.empty()){
        int explored_target_index = targets_to_explore.front();
        targets_to_explore.pop();
        for(list<int>::const_iterator communication_neighbor_iterator = communication_neighbors[explored_target_index].begin(); communication_neighbor_iterator != communication_neighbors[explored_target_index].end(); communication_neighbor_iterator++){
            int communication_neighbor_index = *communication_neighbor_iterator;
            // if the communication neighbor has not been explored yet (distance -1), its distance is set and it is added to the targets to explore
            if(shortest_paths_to_source[communication_neighbor_index] == -1){
                shortest_paths_to_source[communication_neighbor_index] = shortest_paths_to_source[explored_target_index] + 1;
                targets_to_explore.push(communication_neighbor_index);
                next_target_on_shortest_path_to_source[communication_neighbor_index] = explored_target_index;
            }
        }
    }

    // computing shortest paths from all other targets
    for(int target_index = 0; target_index<number_targets; target_index++){
        queue<int> other_targets_to_explore;

        // initializing the queue with the target itself
        shortest_paths[target_index][target_index] = 0;
        next_target_on_shortest_path[target_index][target_index] = target_index;
        other_targets_to_explore.push(target_index);

        while(!other_targets_to_explore.empty()){
            int explored_target_index = other_targets_to_explore.front();
            other_targets_to_explore.pop();
            for(list<int>::const_iterator communication_neighbor_iterator = communication_neighbors[explored_target_index].begin(); communication_neighbor_iterator != communication_neighbors[explored_target_index].end(); communication_neighbor_iterator++){
                int communication_neighbor_index = *communication_neighbor_iterator;
                // if the communication neighbor has not been explored yet (distance -1), its distance is set and it is added to the targets to explore
                if(shortest_paths[target_index][communication_neighbor_index] == -1){
                    shortest_paths[target_index][communication_neighbor_index] = shortest_paths[target_index][explored_target_index] + 1;
                    other_targets_to_explore.push(communication_neighbor_index);
                    next_target_on_shortest_path[target_index][communication_neighbor_index] = explored_target_index;
                }
            }
        }
    }
}
