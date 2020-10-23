#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QDockWidget>
#include <QTableView>
#include <QGridLayout>

#include "data_models.h"

class Sidebar : public QDockWidget
{
    Q_OBJECT
private:

    QTableView* targets_table_view;
    QTableView* parameters_view;
    QTableView* solutions_table_view;

    TargetsTableModel* targets_table_model;
    ParametersModel* parameters_model;
    SolutionsTableModel* solutions_table_model;

    void handleSelectedSolutionChanged(const QItemSelection &selected, const QItemSelection &deselected);

public:
    Sidebar(TargetsTableModel *targets_table_model, ParametersModel *parameters_model, SolutionsTableModel *solutions_table_model, QWidget *parent = nullptr);


signals:
    void selectedSolutionChanged(int solution_index);
};

#endif // SIDEBAR_H
