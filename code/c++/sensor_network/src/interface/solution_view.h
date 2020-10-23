#ifndef SOLUTION_VIEW_H
#define SOLUTION_VIEW_H

#include "data_models.h"

#include <QAbstractItemView>
#include <QPainter>
#include <QScrollBar>
#include <QPaintEvent>
#include <QToolTip>
#include <QMenu>
#include <cmath>

class SolutionView : public QAbstractItemView
{
    Q_OBJECT

public:
    SolutionView(SolutionModel *solution_model, QWidget *parent = nullptr);

    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
    QModelIndex indexAt(const QPoint &point) const override;

    // public methods for zoom
    int getZoom() const;
    void setZoom(int value);
    void setScale(int value);

    void adaptScale();


protected slots:
   void rowsInserted(const QModelIndex &parent, int start, int) override;

protected:
   QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;

   int horizontalOffset() const override;
   int verticalOffset() const override;

   bool isIndexHidden(const QModelIndex &index) const override;

   void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command) override;

   void paintEvent(QPaintEvent *event) override;
   void resizeEvent(QResizeEvent *event) override;
   void scrollContentsBy(int dx, int dy) override;

   QRegion visualRegionForSelection(const QItemSelection &selection) const override;

   void mousePressEvent(QMouseEvent *event) override;
   void mouseMoveEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;

   void wheelEvent(QWheelEvent *event) override;

   void showContextMenu(const QPoint &pos);

   // function for tooltips
   bool viewportEvent(QEvent *event) override;


private:
   void updateGeometries() override;

   int base_scale;
   int icon_size;
   int zoom;

   SolutionModel* solution_model;

   // selection model from the sidebar
   QItemSelectionModel *task_selection_model;

   // private methods to get scales
   int getScale() const;
   pair<int, int> mapToViewportCoordinates(const pair<float, float> coordinates) const;


};

#endif // SOLUTION_VIEW_H
