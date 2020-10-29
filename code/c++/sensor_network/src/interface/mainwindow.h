#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <random>

#include <QMainWindow>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>

#include <data_structures/data_set.h>
#include "sidebar.h"
#include "centralwidget.h"
#include <tools/parsing.h>
#include <algorithms/heuristics.h>
#include <algorithms/metaheuristics.h>
#include <tools/writing.h>
#include "metaheuristic_dialog.h"
#include "metaheuristic_thread.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

private:
    DataSet *data_set;

    void closeEvent(QCloseEvent *event);

    Sidebar *sidebar;
    CentralWidget * central_widget;

    TargetsTableModel* targets_table_model;
    SolutionsTableModel* solutions_table_model;
    ParametersModel* parameters_model;
    SolutionModel* solution_model;

    QString current_data_set_folder_path;

    void setDataSet(DataSet *new_data_set);
    void addSolution(Solution* solution);
    void handleSelectedSolutionChanged(int solution_index);

    void computeHeuristicSolution();
    void launchMetaheuristic();

public:
    MainWindow();
    ~MainWindow();

private slots:
    void loadDataSet();
    void loadSavedDataSet();
    void saveDataSetAs();

};

#endif // MAINWINDOW_H
