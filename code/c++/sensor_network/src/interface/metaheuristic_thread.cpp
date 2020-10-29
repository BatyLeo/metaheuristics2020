#include "metaheuristic_thread.h"

MetaheuristicThread::MetaheuristicThread(int number_iterations, const Solution* initial_solution, float initial_keep_probability, float final_keep_probability, float phi, QObject *parent) :
    QThread(parent), continue_metaheuristic(true),
    number_iterations(number_iterations), initial_solution(initial_solution), initial_keep_probability(initial_keep_probability), final_keep_probability(final_keep_probability), phi(phi)
{

}

void MetaheuristicThread::stopMetaheuristic(){
    continue_metaheuristic = false;
}

bool MetaheuristicThread::getContinueMetaheuristic() const{
    return continue_metaheuristic;
}

void MetaheuristicThread::run(){
    Solution* solution = simulatedAnnealingMetaheuristicForInterface(number_iterations, initial_solution, initial_keep_probability, final_keep_probability, phi,
    [this](){return this->getContinueMetaheuristic();},
    [this](int score, float temperature){emit this->addScoreAndTemperatureValues(score, temperature);});

    emit addMetaheuristicSolution(solution);
}
