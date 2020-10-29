#include "metaheuristic_dialog.h"

MetaheuristicDialog::MetaheuristicDialog(QWidget *parent) : QDialog(parent),
iteration(0), max_temperature(0), max_score(0), iteration_range(2)
{
    setModal(true);
    showMaximized();

    temperature_chart_view = new QtCharts::QChartView(this);
    temperature_chart_view->setRenderHint(QPainter::Antialiasing);

    score_chart_view = new QtCharts::QChartView(this);
    score_chart_view->setRenderHint(QPainter::Antialiasing);

    temperature_chart = new QtCharts::QChart();
    temperature_chart->setTitle("Temperature");
    temperature_chart->setAnimationOptions(QtCharts::QChart::AllAnimations);
    temperature_chart->legend()->hide();
    temperature_chart_view->setChart(temperature_chart);

    score_chart = new QtCharts::QChart();
    score_chart->setTitle("Score");
    score_chart->setAnimationOptions(QtCharts::QChart::AllAnimations);
    score_chart->legend()->hide();
    score_chart_view->setChart(score_chart);

    temperature_series = new QtCharts::QLineSeries();
    temperature_chart->addSeries(temperature_series);
    temperature_x_axis = new QtCharts::QValueAxis();
    temperature_y_axis = new QtCharts::QValueAxis();
    temperature_chart->addAxis(temperature_x_axis,Qt::AlignBottom);
    temperature_chart->addAxis(temperature_y_axis,Qt::AlignLeft);
    temperature_series->attachAxis(temperature_x_axis);
    temperature_series->attachAxis(temperature_y_axis);

    score_series = new QtCharts::QLineSeries();
    score_chart->addSeries(score_series);
    score_x_axis = new QtCharts::QValueAxis();
    score_y_axis = new QtCharts::QValueAxis();
    score_chart->addAxis(score_x_axis,Qt::AlignBottom);
    score_chart->addAxis(score_y_axis,Qt::AlignLeft);
    score_series->attachAxis(score_x_axis);
    score_series->attachAxis(score_y_axis);

    QPushButton* stop_metaheuristic_button = new QPushButton(QString("Stop metaheuristic"), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(temperature_chart_view, 0, 0, 1, 2);
    layout->addWidget(score_chart_view, 1, 0, 1, 2);
    layout->addWidget(stop_metaheuristic_button, 2, 1, 1, 1);
    setLayout(layout);

    connect(stop_metaheuristic_button, &QPushButton::released, this, &MetaheuristicDialog::reject);

}

void MetaheuristicDialog::addValues(int score, float temperature){
    if(temperature > max_temperature){
        max_temperature = temperature;
        temperature_y_axis->setRange(0, max_temperature);
    }
    if(score > max_score){
        max_score = score;
        score_y_axis->setRange(0, max_score);
    }
    if(iteration>iteration_range){
        iteration_range *= 2;
        score_x_axis->setRange(0, iteration_range);
        temperature_x_axis->setRange(0, iteration_range);
    }

    temperature_series->append(iteration, temperature);
    score_series->append(iteration, score);

    iteration++;
}



