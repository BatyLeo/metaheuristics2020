#include "writing.h"

void writeDataSetToFile(string file_location, string file_name, const DataSet* data_set)
{
    int number_targets = data_set->getNumberTargets();

    ofstream file;
    string file_path = file_location+"/"+file_name;
    file.open(file_path);

    file<<0<<" "<<0<<" "<<0<<endl;
    for(int target_index = 0; target_index<number_targets; target_index++){
        file<<target_index+1<<" ";
        file<<data_set->getTargetCoordinates(target_index).first<<" ";
        file<<data_set->getTargetCoordinates(target_index).second<<endl;
    }

    file.close();
}

void writeSolutionToFile(string file_location, string file_name, const Solution* solution)
{
    const DataSet* data_set = solution->getDataSet();
    int number_targets = data_set->getNumberTargets();

    ofstream file;
    string file_path = file_location+"/"+file_name;
    file.open(file_path);

    for(int target_index = 0; target_index<number_targets; target_index++){
        file<<target_index+1<<" "<<solution->getTargetHasSensor(target_index)<<endl;
    }

    file.close();
}
