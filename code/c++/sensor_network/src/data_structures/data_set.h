#ifndef DATA_SET_H
#define DATA_SET_H

#include <vector>
#include <list>
#include <queue>
using namespace std;

class Solution;

class DataSet{
private:

    int number_targets;

    float communication_radius;
    float reception_radius;

    int reception_level;

    vector<pair<float, float> > target_coordinates;

    vector<list<int> > communication_neighbors;
    vector<list<int> > reception_neighbors;

    list<int> source_communication_neighbors;

    vector<vector<int> > shortest_paths;
    vector<int> shortest_paths_to_source;

    vector<vector<int> > next_target_on_shortest_path;
    vector<int> next_target_on_shortest_path_to_source;

    vector<Solution*> solutions;

    void buildCommunicationArcs();
    void buildReceptionArcs();
    void buildShortestPaths();

public:
    DataSet(float communication_radius, float reception_radius, int reception_level, const vector<pair<float, float> > &target_coordinates);
    ~DataSet();

    float getCommunicationRadius() const;
    float getReceptionRadius() const;
    int getNumberTargets() const;
    int getReceptionLevel() const;
    const pair<float, float> &getTargetCoordinates(int target_index) const;

    const list<int>& getCommunicationNeighbors(int target_index) const;
    const list<int>& getReceptionNeighbors(int target_index) const;
    const list<int>& getSourceCommunicationNeighbors() const;

    int getShortestPathToSource(int target_index) const;
    int getNextTargetOnShortestPathToSource(int target_index) const;
    int getShortestPath(int origin_target_index, int destination_target_index) const;
    int getNextTargetOnShortestPath(int origin_target_index, int destination_target_index) const;

    void setCommunicationRadius(float new_communication_radius);
    void setReceptionRadius(float new_reception_radius);
    void setReceptionLevel(int new_reception_level);

    void addSolution(Solution* solution);
    void removeSolution(int solution_index);
    int getNumberSolutions();
    Solution* getSolution(int solution_index);
};

#endif // DATA_SET_H
