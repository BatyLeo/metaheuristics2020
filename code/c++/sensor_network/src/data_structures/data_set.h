#ifndef DATA_SET_H
#define DATA_SET_H

#include <vector>
#include <list>
using namespace std;

class DataSet{
private:

    int number_targets;

    int communication_radius;
    int reception_radius;

    int reception_level;

    vector<pair<float, float> > target_coordinates;

    vector<list<int> > communication_neighbors;
    vector<list<int> > reception_neighbors;

    list<int> source_communication_neighbors;

public:
    DataSet(int communication_radius, int reception_radius, int reception_level, const vector<pair<float, float> > &target_coordinates);
    ~DataSet();

    int getCommunicationRadius() const;
    int getReceptionRadius() const;
    int getNumberTargets() const;
    int getReceptionLevel() const;
    const pair<float, float> &getTargetCoordinates(int target_index) const;

    const list<int>& getCommunicationNeighbors(int target_index) const;
    const list<int>& getReceptionNeighbors(int target_index) const;
    const list<int>& getSourceCommunicationNeighbors() const;
};

#endif // DATA_SET_H
