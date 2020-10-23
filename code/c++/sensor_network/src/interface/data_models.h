#ifndef DATA_MODELS_H
#define DATA_MODELS_H

#include <QAbstractTableModel>

#include <data_structures/solution.h>

class TargetsTableModel : public QAbstractTableModel
{
public:
    TargetsTableModel(DataSet *data_set, QObject *parent =nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setDataSet(DataSet* new_data_set);

private:
    const DataSet *data_set;
};

class ParametersModel : public QAbstractTableModel
{
public:
    ParametersModel(DataSet *data_set, QObject *parent =nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDataSet(DataSet* new_data_set);

private:
    DataSet *data_set;
};

class SolutionsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SolutionsTableModel(DataSet *data_set, QObject *parent =nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setDataSet(DataSet* new_data_set);

private:
    DataSet *data_set;
};

class SolutionModel : public QAbstractTableModel
{
public:
    SolutionModel(Solution *solution, QObject *parent =nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    const Solution* getSolution() const;

    void setSolution(Solution* new_solution);

private:
    Solution *solution;
};


#endif // DATA_MODELS_H
