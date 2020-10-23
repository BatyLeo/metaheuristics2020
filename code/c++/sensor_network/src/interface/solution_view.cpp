#include "solution_view.h"


SolutionView::SolutionView(SolutionModel *solution_model, QWidget *parent) : QAbstractItemView(parent),
    solution_model(solution_model)
{

    icon_size = 5;
    zoom = 100;

    base_scale = 1;

    setAutoScroll(false);
    setModel(solution_model);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SolutionView::customContextMenuRequested, this, &SolutionView::showContextMenu);
}

QModelIndex SolutionView::indexAt(const QPoint &point) const
{
    Q_UNUSED(point)
    return QModelIndex();
}

bool SolutionView::isIndexHidden(const QModelIndex & /*index*/) const
{
    return false;
}

int SolutionView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

QModelIndex SolutionView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers /*modifiers*/)
{
    Q_UNUSED(cursorAction)
    QModelIndex current = currentIndex();

    viewport()->update();
    return current;
}

void SolutionView::paintEvent(QPaintEvent *event)
{

    QStyleOptionViewItem option = viewOptions();

    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), background);
    painter.setPen(foreground);

    painter.save();

    // translate painter to be in accordance with scollbars
    painter.translate(0 - horizontalScrollBar()->value(), 0 - verticalScrollBar()->value());

    const DataSet* data_set = solution_model->getSolution()->getDataSet();
    int number_targets = data_set->getNumberTargets();

    // drawing reception arcs between targets
    painter.setPen(QColor(0, 0, 255));
    for(int target_index = 0; target_index<number_targets; target_index++){
        if(solution_model->getSolution()->getTargetHasSensor(target_index)){
            pair<int, int> target_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_index));
            int target_x = target_viewport_coordinates.first;
            int target_y = target_viewport_coordinates.second;
            for(list<int>::const_iterator target_reception_neighbor_iterator = data_set->getReceptionNeighbors(target_index).begin(); target_reception_neighbor_iterator != data_set->getReceptionNeighbors(target_index).end(); target_reception_neighbor_iterator++){
                int target_reception_neighbor_index = *target_reception_neighbor_iterator;
                if(!solution_model->getSolution()->getTargetHasSensor(target_reception_neighbor_index)){
                    pair<int, int> target_reception_neighbor_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_reception_neighbor_index));
                    int target_reception_neighbor_x = target_reception_neighbor_viewport_coordinates.first;
                    int target_reception_neighbor_y = target_reception_neighbor_viewport_coordinates.second;

                    painter.drawLine(target_x, target_y, target_reception_neighbor_x, target_reception_neighbor_y);
                }
            }
        }
    }

    // drawing communication arcs between targets
    painter.setPen(QColor(255, 0, 0));
    for(int target_index = 0; target_index<number_targets; target_index++){
        if(solution_model->getSolution()->getTargetHasSensor(target_index)){
            pair<int, int> target_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_index));
            int target_x = target_viewport_coordinates.first;
            int target_y = target_viewport_coordinates.second;
            for(list<int>::const_iterator target_communication_neighbor_iterator = data_set->getCommunicationNeighbors(target_index).begin(); target_communication_neighbor_iterator != data_set->getCommunicationNeighbors(target_index).end(); target_communication_neighbor_iterator++){
                int target_communication_neighbor_index = *target_communication_neighbor_iterator;
                if(solution_model->getSolution()->getTargetHasSensor(target_communication_neighbor_index)){
                    pair<int, int> target_communication_neighbor_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_communication_neighbor_index));
                    int target_communication_neighbor_x = target_communication_neighbor_viewport_coordinates.first;
                    int target_communication_neighbor_y = target_communication_neighbor_viewport_coordinates.second;

                    painter.drawLine(target_x, target_y, target_communication_neighbor_x, target_communication_neighbor_y);
                }
            }
        }
    }

    // drawing targets
    painter.setPen(QColor(0, 0, 0));
    for(int target_index = 0; target_index<number_targets; target_index++){
        pair<int, int> target_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_index));
        int target_x = target_viewport_coordinates.first;
        int target_y = target_viewport_coordinates.second;

        // !!! to improve
        if(solution_model->getSolution()->getTargetHasSensor(target_index)){
            painter.setBrush(QBrush(QColor(255, 0, 0)));
            painter.drawRect(target_x - icon_size, target_y - icon_size, 2*icon_size, 2*icon_size);
        } else{
            painter.setBrush(QBrush(QColor(255, 255, 255)));
            painter.drawEllipse(QPoint(target_x, target_y), icon_size, icon_size);
        }
        //painter.drawText(target_x, target_y, QString("%1").arg(target_index+1));
    }

    painter.restore();
}

void SolutionView::resizeEvent(QResizeEvent * /* event */)
{
    updateGeometries();
}


void SolutionView::scrollContentsBy(int dx, int dy)
{
    viewport()->scroll(dx, dy);
    viewport()->update();
}


void SolutionView::scrollTo(const QModelIndex &index, ScrollHint)
{
    if(index.isValid()){
        int target_index = index.row();

        const DataSet* data_set = solution_model->getSolution()->getDataSet();

        pair<int, int> target_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_index));
        int target_x = target_viewport_coordinates.first;
        int target_y = target_viewport_coordinates.second;

        // the scroll is only done if the center point of the task is not inside the viewport
        bool center_is_visible = (target_x>=horizontalScrollBar()->value())&&(target_x<=horizontalScrollBar()->value()+viewport()->width())&&(target_y>=verticalScrollBar()->value())&&(target_y<=verticalScrollBar()->value()+viewport()->height());
        if(!center_is_visible){
            horizontalScrollBar()->setValue(target_x - viewport()->width()/2);
            verticalScrollBar()->setValue(target_y - viewport()->height()/2);

            viewport()->update();
        }
    }
}

void SolutionView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    Q_UNUSED(rect)
    Q_UNUSED(command);

    viewport()->update();
}

void SolutionView::updateGeometries()
{
    // to improve
    const DataSet* data_set = solution_model->getSolution()->getDataSet();
    int number_targets = data_set->getNumberTargets();
    float max_x = 0;
    float max_y = 0;
    for(int target_index = 0; target_index<number_targets; target_index++){
        max_x = max(max_x, data_set->getTargetCoordinates(target_index).first);
        max_y = max(max_y, data_set->getTargetCoordinates(target_index).second);
    }

    int total_width = round(max_x*getScale());
    total_width = max(viewport()->width(), total_width);
    int total_height = round(max_y*getScale());
    total_height = max(viewport()->height(), total_height);

    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0, total_width - viewport()->width());
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setRange(0, total_height - viewport()->height());
}

int SolutionView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

QRect SolutionView::visualRect(const QModelIndex &index) const
{
    if(index.isValid()){

        int target_index = index.row();

        const DataSet* data_set = solution_model->getSolution()->getDataSet();

        pair<int, int> target_viewport_coordinates = mapToViewportCoordinates(data_set->getTargetCoordinates(target_index));
        int target_x = target_viewport_coordinates.first;
        int target_y = target_viewport_coordinates.second;

        QRect rectangle = QRect(target_x - icon_size, target_y - icon_size, 2*icon_size, 2*icon_size);
        return rectangle;

    }
    return QRect();
}

void SolutionView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
}

QRegion SolutionView::visualRegionForSelection(const QItemSelection &selection) const
{
    Q_UNUSED(selection)
    return QRegion();
}

void SolutionView::mousePressEvent(QMouseEvent *event)
{
    QAbstractItemView::mousePressEvent(event);
}


void SolutionView::mouseMoveEvent(QMouseEvent *event)
{
    QAbstractItemView::mouseMoveEvent(event);
}


void SolutionView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void SolutionView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier){
        int event_x = event->pos().x();
        int event_y = event->pos().y();
        double mouse_normalized_x = (horizontalScrollBar()->value()+event_x)/(double)getScale();
        double mouse_normalized_y = (verticalScrollBar()->value()+event_y)/(double)getScale();

        setZoom(getZoom()+event->delta()/12);

        horizontalScrollBar()->setValue(round(mouse_normalized_x*getScale())-event_x);
        verticalScrollBar()->setValue(round(mouse_normalized_y*getScale())-event_y);
        event->accept();
    } else{
        QAbstractItemView::wheelEvent(event);
    }
}

bool SolutionView::viewportEvent(QEvent *event)
{
    /*if(event->type() == QEvent::ToolTip){
        QHelpEvent *help_event = static_cast<QHelpEvent *>(event);

        QModelIndex index = indexAt(help_event->pos());
        if(index.isValid()){
            int row = index.row();
            QString tool_tip = QString::fromStdString(data_set->getTaskName(index.row()));
            QVariant beginning(solution_model->getTaskDate(row));
            QVariant end(solution_model->getTaskDate(row)+data_set->getTaskDuration(row));
            tool_tip+=QString("\n %1 - %2").arg(beginning.toString(), end.toString());
            QToolTip::showText(help_event->globalPos(), tool_tip);
        } else{
            QToolTip::hideText();
            event->ignore();
        }
    }*/
    return QAbstractItemView::viewportEvent(event);
}

void SolutionView::showContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos)
    /*QModelIndex index_clicked_task = indexAt(pos);

    if(index_clicked_task.isValid() && manual_modifications_enabled)
    {
        int task = index_clicked_task.row();

        QMenu *context_menu = new QMenu("", this);

        if(solution_model->getTaskIsAnchored(task)){
            QAction *remove_anchorage = new QAction("Remove anchorage", this);
            connect(remove_anchorage, &QAction::triggered, [&, task](){this->setTaskAnchorageAndUpdate(task, false);});
            context_menu->addAction(remove_anchorage);
        } else{
            QAction *anchor = new QAction("Anchor", this);
            connect(anchor, &QAction::triggered, [&, task](){this->setTaskAnchorageAndUpdate(task, true);});
            context_menu->addAction(anchor);
        }

        context_menu->exec(mapToGlobal(pos));

    }*/
}


void SolutionView::setScale(int value)
{
    if(value<1 || value >viewport()->width()){
        return;
    }
    base_scale = value;
    updateGeometries();
    viewport()->update();
}

void SolutionView::adaptScale(){
    // to improve
    const DataSet* data_set = solution_model->getSolution()->getDataSet();
    int number_targets = data_set->getNumberTargets();
    float max_x = 1;
    float max_y = 1;
    for(int target_index = 0; target_index<number_targets; target_index++){
        max_x = max(max_x, data_set->getTargetCoordinates(target_index).first);
        max_y = max(max_y, data_set->getTargetCoordinates(target_index).second);
    }

    base_scale = max(1, (int)round(min((viewport()->height()-2*icon_size)/max_y, (viewport()->width()-2*icon_size)/max_x)));
}


int SolutionView::getZoom() const
{
    return zoom;
}

void SolutionView::setZoom(int value)
{
    if(value<=0){
        return;
    }

    int new_scale = round(base_scale * value/(double)100);

    // testing if the new value gives proper time scale and task height
    if(1<=new_scale && new_scale<viewport()->width()){
        zoom = value;
        updateGeometries();
        viewport()->update();
    }
}

int SolutionView::getScale() const
{
    return round(base_scale * zoom/(double)100);
}

pair<int, int> SolutionView::mapToViewportCoordinates(const pair<float, float> coordinates) const{
    return pair<int, int>(icon_size + round(coordinates.first*getScale()), icon_size + round(coordinates.second*getScale()));
}

