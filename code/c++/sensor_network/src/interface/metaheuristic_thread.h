#ifndef METAHEURISTIC_THREAD_H
#define METAHEURISTIC_THREAD_H

#include <QThread>

#include <algorithms/metaheuristics.h>

class MetaheuristicThread : public QThread
{
    Q_OBJECT

public:
    MetaheuristicThread(int number_iterations, const Solution* initial_solution, float initial_keep_probability, float final_keep_probability, float phi, QObject *parent = nullptr);

    void stopMetaheuristic();
    bool getContinueMetaheuristic() const;

    void run() override;

private:
    bool continue_metaheuristic;

    int number_iterations;
    const Solution* initial_solution;
    float initial_keep_probability;
    float final_keep_probability;
    float phi;

signals:
    void addScoreAndTemperatureValues(int score, float temperature);
    void addMetaheuristicSolution(Solution* solution);
};

#endif // METAHEURISTIC_THREAD_H
