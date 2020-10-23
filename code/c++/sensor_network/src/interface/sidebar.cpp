#include "sidebar.h"

Sidebar::Sidebar(TargetsTableModel* targets_table_model, ParametersModel* parameters_model, SolutionsTableModel* solutions_table_model, QWidget *parent) : QDockWidget(parent),
targets_table_model(targets_table_model), parameters_model(parameters_model), solutions_table_model(solutions_table_model)
{
    QWidget *container =  new QWidget(this);
    QGridLayout *layout = new QGridLayout(container);

    targets_table_view = new QTableView(container);
    parameters_view = new QTableView(container);
    solutions_table_view = new QTableView(container);

    targets_table_view->setModel(targets_table_model);
    parameters_view->setModel(parameters_model);
    solutions_table_view->setModel(solutions_table_model);

    solutions_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    solutions_table_view->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(targets_table_view, 0, 0, 1, 1);
    layout->addWidget(parameters_view, 0, 1, 1, 1);
    layout->addWidget(solutions_table_view, 1, 0, 2, 2);

    container->setLayout(layout);

    setWidget(container);

    connect(solutions_table_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Sidebar::handleSelectedSolutionChanged);
}

void Sidebar::handleSelectedSolutionChanged(const QItemSelection &selected, const QItemSelection &deselected){
    Q_UNUSED(deselected)

    if(selected.length()){
        QModelIndex selected_index = selected.indexes().back();

        if(selected_index.isValid()){
            int solution_index = selected_index.row();
            emit selectedSolutionChanged(solution_index);
        }
    }
}
