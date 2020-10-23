#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "solution_view.h"

#include <QWidget>
#include <QGridLayout>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(SolutionModel* solution_model, QWidget *parent = nullptr);
    ~CentralWidget();

private:
    SolutionModel* solution_model;
    SolutionView* solution_view;

};

#endif // CENTRALWIDGET_H
