#include "solution.h"

#include <algorithm>
#include <queue>
using namespace std;

Solution::Solution(const DataSet* data_set) :
data_set(data_set)
{
    score = 0;
    target_has_sensor = vector<bool>(data_set->getNumberTargets(), false);
}

Solution::Solution(const DataSet* data_set, const vector<bool> &sensor_placement) :
data_set(data_set)
{
    score = 0;

    int number_targets = data_set->getNumberTargets();
    if(sensor_placement.size() != number_targets){
        throw "number of targets incompatible with data set";
    }

    target_has_sensor = vector<bool>(number_targets);
    for(int target_index = 0; target_index<number_targets; target_index++){
        if(sensor_placement[target_index]){
            setTargetHasSensor(target_index, true);
        }
    }
}

Solution::~Solution(){

}

bool Solution::getTargetHasSensor(int target_index) const{
    return target_has_sensor[target_index];
}

int Solution::getScore() const{
    return score;
}

const DataSet* Solution::getDataSet() const{
    return data_set;
}

void Solution::setTargetHasSensor(int target_index, bool value){
    bool previous_value = target_has_sensor[target_index];
    target_has_sensor[target_index] = value;
    int score_change = value - previous_value;
    score = score + score_change;
}


bool Solution::checkAdmissible() const{
    int number_targets = data_set->getNumberTargets();

    // checking that all targets have the correct reception level
    int reception_level = data_set->getReceptionLevel();
    for(int target_index = 0; target_index<number_targets; target_index++){
        int covering_sensor_count = 0;
        // iterating over reception neighbors to count covering sensors
        for(list<int>::const_iterator reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); reception_neighbor_iterator++){
            int reception_neighbor_index = *reception_neighbor_iterator;
            if(target_has_sensor[reception_neighbor_index]){
                covering_sensor_count++;
            }
            if(covering_sensor_count >= reception_level){
                break;
            }
        }

        if(covering_sensor_count < reception_level){
            return false;
        }
    }

    // checking that all placed sensors are accessible from source
    vector<bool> is_connected_to_source(number_targets, false);
    queue<int> targets_to_explore = queue<int>();
    // adding source communication neighbors that have sensors to the targets to explore
    for(list<int>::const_iterator source_communication_neighbor_iterator = data_set->getSourceCommunicationNeighbors().begin(); source_communication_neighbor_iterator != data_set->getSourceCommunicationNeighbors().end(); source_communication_neighbor_iterator++){
        int source_communication_neighbor_index = *source_communication_neighbor_iterator;

        if(target_has_sensor[source_communication_neighbor_index]){
            targets_to_explore.push(source_communication_neighbor_index);
            is_connected_to_source[source_communication_neighbor_index] = true;
        }
    }
    // exploring targets
    while(!targets_to_explore.empty()){
        int target_index = targets_to_explore.front();
        targets_to_explore.pop();
        const list<int> & target_communication_neighbors = data_set->getCommunicationNeighbors(target_index);

        for(list<int>::const_iterator target_communication_neighbor_iterator = target_communication_neighbors.begin(); target_communication_neighbor_iterator != target_communication_neighbors.end(); target_communication_neighbor_iterator++){
            int target_communication_neighbor_index = *target_communication_neighbor_iterator;

            // adding targets that have sensor and were not yet connected to source to the targets to explore
            if(target_has_sensor[target_communication_neighbor_index] && !is_connected_to_source[target_communication_neighbor_index]){
                targets_to_explore.push(target_communication_neighbor_index);
                is_connected_to_source[target_communication_neighbor_index] = true;
            }
        }
    }

    // comparing targets with sensors to those connected to source
    for(int target_index = 0; target_index<number_targets; target_index++){
        // if a target has a sensor but is not connected to source, the solution is not admissible
        if(target_has_sensor[target_index] && !is_connected_to_source[target_index]){
            return false;
        }
    }

    return true;
}

