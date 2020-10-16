#include "data_set.h"

DataSet::DataSet(int communication_radius, int reception_radius, int reception_level, const vector<pair<float, float> > &target_coordinates) :
communication_radius(communication_radius), reception_radius(reception_radius), reception_level(reception_level) ,target_coordinates(target_coordinates)
{
    number_targets = target_coordinates.size();

    // building communication and reception arcs
    communication_neighbors = vector<list<int> >(number_targets, list<int>());
    reception_neighbors = vector<list<int> >(number_targets, list<int>());
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

            // adding reception arc
            if(square_distance <= reception_radius*reception_radius){
                reception_neighbors[target_index].push_back(other_target_index);
            }
        }

        // adding connection arc from source
        float square_distance_to_source = target_coordinates[target_index].first*target_coordinates[target_index].first + target_coordinates[target_index].second*target_coordinates[target_index].second;
        if(square_distance_to_source<=communication_radius*communication_radius){
            source_communication_neighbors.push_back(target_index);
        }
    }
}

DataSet::~DataSet(){

}

int DataSet::getCommunicationRadius() const{
    return communication_radius;
}

int DataSet::getReceptionRadius() const{
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

