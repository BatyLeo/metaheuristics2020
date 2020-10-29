#ifndef METAHEURISTIC_DIALOG_H
#define METAHEURISTIC_DIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QGridLayout>
#include <QPushButton>
#include <QCoreApplication>

#include <algorithms/metaheuristics.h>

class MetaheuristicDialog : public QDialog
{
    Q_OBJECT

public:
    MetaheuristicDialog(QWidget *parent = nullptr);

    void addValues(int score, float temperature);

private:

    int iteration;
    float max_temperature;
    int max_score;
    int iteration_range;

    QtCharts::QLineSeries *temperature_series;
    QtCharts::QLineSeries *score_series;

    QtCharts::QChart* temperature_chart;
    QtCharts::QChart* score_chart;

    QtCharts::QChartView* temperature_chart_view;
    QtCharts::QChartView* score_chart_view;

    QtCharts::QValueAxis* temperature_x_axis;
    QtCharts::QValueAxis* temperature_y_axis;
    QtCharts::QValueAxis* score_x_axis;
    QtCharts::QValueAxis* score_y_axis;

};


#endif // METAHEURISTIC_DIALOG_H
