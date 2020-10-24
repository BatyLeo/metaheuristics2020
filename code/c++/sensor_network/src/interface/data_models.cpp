#include "data_models.h"

TargetsTableModel::TargetsTableModel(DataSet *data_set, QObject *parent) : QAbstractTableModel(parent),
    data_set(data_set)
{

}

QVariant TargetsTableModel::data(const QModelIndex &index, int role) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(!index.isValid()){
        return QVariant();
    }
    int target_index = index.row();
    int column = index.column();
    switch(column){
    case 0:
        return QVariant(data_set->getTargetCoordinates(target_index).first);
    case 1:
        return QVariant(data_set->getTargetCoordinates(target_index).second);
    default :
        return QVariant();

    }
    return QVariant();
}

QVariant TargetsTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole){
            return QVariant();
    }
    if(orientation == Qt::Vertical){
        return QString("%1").arg(section+1);
    } else if(orientation == Qt::Horizontal){
        switch(section){
        case 0:
            return QVariant("x");
        case 1:
            return QVariant("y");
        default :
            return QVariant();

        }

    }
    return QVariant();
}

int TargetsTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return data_set->getNumberTargets();
}

int TargetsTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

void TargetsTableModel::setDataSet(DataSet *new_data_set){
    beginResetModel();
    data_set = new_data_set;
    endResetModel();
}

ParametersModel::ParametersModel(DataSet *data_set, QObject *parent) : QAbstractTableModel(parent),
    data_set(data_set)
{

}

QVariant ParametersModel::data(const QModelIndex &index, int role) const {
    if(role != Qt::DisplayRole){
            return QVariant();
    }
    if(!index.isValid()){
        return QVariant();
    }
    int row = index.row();
    switch(row){
    case 0:
        return QVariant(data_set->getCommunicationRadius());
    case 1:
        return QVariant(data_set->getReceptionRadius());
    case 2:
        return QVariant(data_set->getReceptionLevel());
    default :
        return QVariant();

    }
    return QVariant();
}

QVariant ParametersModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole){
            return QVariant();
    }
    if(orientation == Qt::Horizontal){
        return QVariant();
    } else if(orientation == Qt::Vertical){
        switch(section){
        case 0:
            return QVariant("Communication radius");
        case 1:
            return QVariant("Reception radius");
        case 2:
            return QVariant("Reception level");
        default :
            return QVariant();

        }

    }
    return QVariant();
}

int ParametersModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 3;
}

int ParametersModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags ParametersModel::flags(const QModelIndex &index) const {
    if(!index.isValid()){
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool ParametersModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(role != Qt::EditRole){
        return false;
    }
    Q_UNUSED(index)
    /*if(index.isValid()){
        int row = index.row();
        bool data_changed = false;
        switch(column){
        case 0:
           data_changed = parameters_set->setTaskLengthening(task, max(0., value.toDouble()));
            if(data_changed){
                emit dataChanged(index,index, {Qt::DisplayRole, Qt::EditRole});
            }
            return data_changed;
        case 1:
            data_changed = parameters_set->setTaskAnchoringWeight(task, max(0, value.toInt()));
            if(data_changed){
                emit dataChanged(index,index, {Qt::DisplayRole, Qt::EditRole});
            }
            return data_changed;
        default:
            return false;
        }
    }*/
    return false;
}

void ParametersModel::setDataSet(DataSet *new_data_set){
    beginResetModel();
    data_set = new_data_set;
    endResetModel();
}

SolutionsTableModel::SolutionsTableModel(DataSet *data_set, QObject *parent) : QAbstractTableModel(parent),
    data_set(data_set)
{

}

QVariant SolutionsTableModel::data(const QModelIndex &index, int role) const {
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(!index.isValid()){
        return QVariant();
    }
    int solution_index = index.row();
    int column = index.column();
    switch(column){
    case 0:
        return QVariant(data_set->getSolution(solution_index)->getScore());
    default :
        return QVariant();

    }
    return QVariant();
}

QVariant SolutionsTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole){
            return QVariant();
    }
    if(orientation == Qt::Vertical){
        return QString("%1").arg(section+1);
    } else if(orientation == Qt::Horizontal){
        switch(section){
        case 0:
            return QVariant("Score");
        default :
            return QVariant();

        }

    }
    return QVariant();
}

int SolutionsTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return data_set->getNumberSolutions();
}

int SolutionsTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

bool SolutionsTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent)
    Q_UNUSED(row)

    for(int i = 0; i<count; i++ ){
        data_set->addSolution(new Solution(data_set));
    }

    return false;
}

bool SolutionsTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent)

    for(int i = 0; i<count; i++){
        data_set->removeSolution(row);
    }
    return false;
}

void SolutionsTableModel::setDataSet(DataSet *new_data_set){
    beginResetModel();
    data_set = new_data_set;
    endResetModel();
}

void SolutionsTableModel::addSolution(Solution* solution){
    beginInsertRows(QModelIndex(), data_set->getNumberSolutions(), data_set->getNumberSolutions());
    data_set->addSolution(solution);
    endInsertRows();
}

SolutionModel::SolutionModel(Solution *solution, QObject *parent) : QAbstractTableModel(parent),
    solution(solution)
{

}

QVariant SolutionModel::data(const QModelIndex &index, int role) const {
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(!index.isValid()){
        return QVariant();
    }
    int target_index = index.row();
    int column = index.column();
    switch(column){
    case 0:
        return QVariant(solution->getTargetHasSensor(target_index));
    default :
        return QVariant();

    }
    return QVariant();
}

QVariant SolutionModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole){
            return QVariant();
    }
    if(orientation == Qt::Vertical){
        return QString("%1").arg(section+1);
    } else if(orientation == Qt::Horizontal){
        switch(section){
        case 0:
            return QVariant("Has sensor");
        default :
            return QVariant();

        }

    }
    return QVariant();
}

int SolutionModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return solution->getDataSet()->getNumberTargets();
}

int SolutionModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

const Solution* SolutionModel::getSolution() const{
    return solution;
}

void SolutionModel::setSolution(Solution* new_solution){
    beginResetModel();
    solution = new_solution;
    endResetModel();
}

