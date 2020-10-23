#include "centralwidget.h"

CentralWidget::CentralWidget(SolutionModel* solution_model, QWidget *parent) : QWidget(parent),
solution_model(solution_model)
{
    QGridLayout* layout = new QGridLayout(this);

    solution_view = new SolutionView(solution_model, this);

    layout->addWidget(solution_view);

    solution_view->adaptScale();
    connect(solution_model, &SolutionModel::modelReset, solution_view, &SolutionView::adaptScale);
}

CentralWidget::~CentralWidget(){

}
