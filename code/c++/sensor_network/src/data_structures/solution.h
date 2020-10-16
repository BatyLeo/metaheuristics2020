#ifndef SOLUTION_H
#define SOLUTION_H

#include "data_set.h"

#include <vector>
using namespace std;

class Solution{
private:

    vector<bool> target_has_sensor;
    int score;

    const DataSet* data_set;

public:
    Solution(const DataSet* data_set);
    ~Solution();

    const DataSet* getDataSet() const;

    bool getTargetHasSensor(int target_index) const;
    int getScore() const;

    void setTargetHasSensor(int target_index, bool value);

    bool checkAdmissible() const;
};

#endif // SOLUTION_H
