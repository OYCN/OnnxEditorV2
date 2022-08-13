/**
 * @file view.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/view.h"

#include <glog/logging.h>

#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollBar>

#include "gui/graph/scene.h"

namespace gui {
namespace graph {

View::View(QWidget *parent) : QGraphicsView{parent} {
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHint(QPainter::Antialiasing);
  // setMouseTracking(true);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  mScene = new Scene(mCfg, this);
  setScene(mScene);

  updateCfg();
}

void View::updateCfg() {
  setBackgroundBrush(mCfg.view.mBackgroundColor);
  update();
  mScene->updateCfg();
}

Scene *View::getScene() {
  CHECK_NOTNULL(mScene);
  return mScene;
}

void View::drawBackground(QPainter *painter, const QRectF &r) {
  QGraphicsView::drawBackground(painter, r);

  auto drawGrid = [&](double gridStep) {
    QRect windowRect = rect();
    QPointF tl = mapToScene(windowRect.topLeft());
    QPointF br = mapToScene(windowRect.bottomRight());

    double left = std::floor(tl.x() / gridStep - 0.5);
    double right = std::floor(br.x() / gridStep + 1.0);
    double bottom = std::floor(tl.y() / gridStep - 0.5);
    double top = std::floor(br.y() / gridStep + 1.0);

    // vertical lines
    for (int xi = static_cast<int>(left); xi <= static_cast<int>(right); ++xi) {
      QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep,
                  top * gridStep);

      painter->drawLine(line);
    }

    // horizontal lines
    for (int yi = static_cast<int>(bottom); yi <= static_cast<int>(top); ++yi) {
      QLineF line(left * gridStep, yi * gridStep, right * gridStep,
                  yi * gridStep);
      painter->drawLine(line);
    }
  };

  QBrush bBrush = backgroundBrush();

  QPen pfine(mCfg.view.mBackgroundFineGridColor, 1.0);

  painter->setPen(pfine);
  drawGrid(15);

  QPen p(mCfg.view.mBackgroundCoarseGridColor, 1.0);

  painter->setPen(p);
  drawGrid(150);
}

void View::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      setDragMode(QGraphicsView::RubberBandDrag);
      break;

    default:
      break;
  }

  QGraphicsView::keyPressEvent(event);
}

void View::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      setDragMode(QGraphicsView::ScrollHandDrag);
      break;

    default:
      break;
  }
  QGraphicsView::keyReleaseEvent(event);
}

void View::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);
  if (event->button() == Qt::LeftButton) {
    click_pos_ = mapToScene(event->pos());
  }
}

void View::mouseMoveEvent(QMouseEvent *event) {
  QGraphicsView::mouseMoveEvent(event);
  if (scene()->mouseGrabberItem() == nullptr &&
      event->buttons() == Qt::LeftButton) {
    // Make sure shift is not being pressed
    if ((event->modifiers() & Qt::ShiftModifier) == 0) {
      QPointF difference = click_pos_ - mapToScene(event->pos());
      setSceneRect(sceneRect().translated(difference.x(), difference.y()));
    }
  }
}

void View::wheelEvent(QWheelEvent *event) {
  QPoint delta = event->angleDelta();

  if (delta.y() == 0) {
    event->ignore();
    return;
  }

  double const d = delta.y() / std::abs(delta.y());

  if (d > 0.0)
    scaleUp();
  else
    scaleDown();
}

void View::scaleUp() {
  double const step = 1.2;
  double const factor = std::pow(step, 1.0);

  QTransform t = transform();

  if (t.m11() > 2.0) return;

  scale(factor, factor);
}

void View::scaleDown() {
  double const step = 1.2;
  double const factor = std::pow(step, -1.0);

  scale(factor, factor);
}

void View::setCfg(config::Ui cfg) {
  mCfg = cfg;
  updateCfg();
}

void View::expand(qreal f) {
  QRectF rectTmp = scene()->sceneRect();
  QPointF ptTopLeft = rectTmp.topLeft();
  QPointF ptBottomRight = rectTmp.bottomRight();
  QPointF ptW_H = f * QPointF(rect().width(), rect().height());
  ptTopLeft -= ptW_H;
  ptBottomRight += ptW_H;
  rectTmp.setTopLeft(ptTopLeft);
  rectTmp.setBottomRight(ptBottomRight);
  scene()->setSceneRect(rectTmp);
  update();
}

void View::center() {
  QScrollBar *hBar = horizontalScrollBar();
  QScrollBar *vBar = verticalScrollBar();
  hBar->setValue(hBar->minimum() + (hBar->maximum() - hBar->minimum()) / 2);
  vBar->setValue(vBar->minimum() + (vBar->maximum() - vBar->minimum()) / 2);
  update();
}

}  // namespace graph
}  // namespace gui
