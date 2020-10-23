// #include <QApplication>
#include <ctime>
#include <cstdlib>

//#include <tools/parsing.h>
//#include <random>
//#include <algorithm>
//#include <algorithms/heuristics.h>
#include <interface/mainwindow.h>

#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    /*string file_name = "/home/luca/Documents/MPRO/Projet_meta/captANOR150_7_4.dat";
    vector<pair<float, float> > target_coordinates = parseCoordinates(file_name);

    int communication_radius = 3;
    int reception_radius = 2;
    int reception_level = 2;

    DataSet data_set(communication_radius, reception_radius, reception_level, target_coordinates);

    int number_targets = data_set.getNumberTargets();

    vector<int> seed_vector = vector<int>(number_targets*reception_level, 0);
    for(int target_index = 0; target_index<number_targets; target_index++){
        for(int i = 0; i<reception_level; i++){
            seed_vector[target_index*reception_level + i] = target_index;
        }
    }
    int min_score = number_targets;

    for(int iter = 0; iter<200; iter++){
        shuffle(seed_vector.begin(), seed_vector.end(), std::default_random_engine(0));

        Solution* solution = shortestPathsHeuristic(&data_set, seed_vector);

        cout<<"Score: "<<solution->getScore();

        //for(int target_index = 0; target_index<number_targets; target_index++){
        //    cout<<target_index<<": "<<solution->getTargetHasSensor(target_index)<<endl;
        //}

        cout<<", admissibility: "<<solution->checkAdmissible()<<endl;

        if(solution->getScore()<min_score){
            min_score = solution->getScore();
        }

        delete solution;
    }

    cout<<"best score: "<<min_score<<endl;

    //std::srand ( unsigned ( std::time(0) ) );*/

    QApplication app(argc, argv);

    MainWindow main_window;
    main_window.showMaximized();

    return app.exec();

    return 0;

}
