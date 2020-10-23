#include "parsing.h"


string fileNameToString(string file_name){
    string file_string;
    char character;
    ifstream file_stream(file_name);
    if(!file_stream){
        cerr<< file_name << "file not found";
        exit(1);
    }
    while( file_stream.get(character)){
        file_string += character;
    }
    file_stream.close();

    return file_string;
}


vector<pair<float, float> > parseCoordinates(string file_name)
{
    string file_string = fileNameToString(file_name);
    stringstream file_stream(file_string);
    string line = "";
    stringstream line_stream;

    vector<pair<float, float> > target_coordinates;
    getline(file_stream, line);

    while(getline(file_stream, line)){
        int target_index;
        float x_coordinate;
        float y_coordinate;

        line_stream = stringstream(line);
        line_stream >> target_index;
        line_stream >> x_coordinate;
        line_stream >> y_coordinate;

        target_coordinates.push_back(pair<float, float>(x_coordinate, y_coordinate));
    }

    return target_coordinates;
}

