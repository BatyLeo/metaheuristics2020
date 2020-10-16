#define CATCH_CONFIG_MAIN
#include <algorithms/heuristics.h>
#include <algorithms/metaheuristics.h>
#include "catch.hpp"
#include <iostream>
using namespace std;



TEST_CASE( "Heuristics", "" ) {
    SECTION("ShortestPathsHeuristic") {

        int number_targets = 6;
        int communication_radius = 3;
        int reception_radius = 2;
        int reception_level = 2;

        vector<pair<float, float> > target_coordinates = vector<pair<float, float> >(number_targets);

        target_coordinates[0] = pair<float, float>(1,1);
        target_coordinates[1] = pair<float, float>(1,3);
        target_coordinates[2] = pair<float, float>(2,2);
        target_coordinates[3] = pair<float, float>(3,1);
        target_coordinates[4] = pair<float, float>(4,2);
        target_coordinates[5] = pair<float, float>(5,1);

        DataSet data_set(communication_radius, reception_radius, reception_level, target_coordinates);

        vector<int> seed_vector = vector<int>({0,1,2,3,4,5,0,1,2,3,4,5});

        Solution* solution = shortestPathsHeuristic(&data_set, seed_vector);

        for(int i = 0; i<number_targets; i++){
            cout<<i<<" "<<solution->getTargetHasSensor(i)<<endl;
        }

        REQUIRE(solution->checkAdmissible() == true);

        solution->setTargetHasSensor(0, false);

        REQUIRE(solution->checkAdmissible() == false);

    }

}

/*TEST_CASE( "Metaheuristic", "" ) {
    SECTION("ResourceFlow neighborhood") {
        ResourceFlow resource_flow(4, 2);
        resource_flow.changeFlow(0, 1, 0, 1);
        resource_flow.changeFlow(0, 2, 0, 1);
        resource_flow.changeFlow(1, 2, 0, 1);
        resource_flow.changeFlow(2, 3, 0, 2);

        resource_flow.changeFlow(0, 1, 1, 2);
        resource_flow.changeFlow(1, 2, 1, 1);
        resource_flow.changeFlow(1, 3, 1, 1);
        resource_flow.changeFlow(2, 3, 1, 1);

        REQUIRE(resource_flow.checkConservationOfFlow() == true);

        removeTaskFromFlowGreedyRandomRepair(resource_flow, 1);

        REQUIRE(resource_flow.checkConservationOfFlow() == true);

        vector<int> task_consumption(2, 0);
        task_consumption[0] = 1; task_consumption[1] = 2;
        list<pair<int, int> > insertion_cut;
        insertion_cut.push_back(pair<int, int>(2, 3)); insertion_cut.push_back(pair<int, int>(0, 3));
        insertTaskInFlowGreedyRandomRepair(resource_flow, 1,insertion_cut , task_consumption);

        REQUIRE(resource_flow.checkConservationOfFlow() == true);

        REQUIRE(((resource_flow.getFlow(0, 1, 0)+resource_flow.getFlow(2, 1, 0)) == task_consumption[0]));
        REQUIRE(((resource_flow.getFlow(0, 1, 1)+resource_flow.getFlow(2, 1, 1)) == task_consumption[1]));

        ResourceFlow resource_flow2(4, 2);
        resource_flow2.changeFlow(0, 1, 0, 1);
        resource_flow2.changeFlow(0, 2, 0, 1);
        resource_flow2.changeFlow(1, 2, 0, 1);
        resource_flow2.changeFlow(2, 3, 0, 2);

        resource_flow2.changeFlow(0, 1, 1, 2);
        resource_flow2.changeFlow(1, 2, 1, 1);
        resource_flow2.changeFlow(1, 3, 1, 1);
        resource_flow2.changeFlow(2, 3, 1, 1);

        REQUIRE(resource_flow2.checkConservationOfFlow() == true);

        removeTaskFromFlowGreedyRandomRepair(resource_flow2, 1);

        REQUIRE(resource_flow2.checkConservationOfFlow() == true);

        insertTaskInFlowGreedyRandomRepair(resource_flow2, 1,insertion_cut , task_consumption);

        REQUIRE(resource_flow2.checkConservationOfFlow() == true);

        REQUIRE(((resource_flow.getFlow(0, 1, 0)+resource_flow.getFlow(2, 1, 0)) == task_consumption[0]));
        REQUIRE(((resource_flow.getFlow(0, 1, 1)+resource_flow.getFlow(2, 1, 1)) == task_consumption[1]));
    }
}*/

