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

#include <QGraphicsItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>

#include "gui/graph/scene.h"

namespace gui {
namespace graph {

View::View(QWidget *parent) : QGraphicsView{parent}, ctx_(this) {
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHint(QPainter::Antialiasing);

  // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  ctx_.top_widget = this;

  scene_ = new Scene(ctx_, this);
  setScene(scene_);

  setBackgroundBrush(ctx_.ui.view.mBackgroundColor);
  update();
}

bool View::closeFile() {
  scene_->clear();
  return true;
}

bool View::loadFile(QString path) {
  // onnx2graph will reset ctx, if not switch ctx obj, we will clear scene
  // before process it
  closeFile();
  {
    bool ret = scene_->getGraph()->openOnnx(path.toStdString());
    if (!ret) {
      QMessageBox::critical(this, tr("Open Error"), tr("open failed"),
                            QMessageBox::Ok);
      return false;
    }
  }
  scene_->loadGraph();
  {
    auto ret = scene_->layout();
    auto rect = ret.first;
    auto pt = ret.second;
    rect.adjust(0, -50, 0, 50);
    setSceneRect(rect);
    setScale(1.5);
    centerOn(pt);
  }
  return true;
}

bool View::saveFile(QString path, bool overwrite) {
  bool ret = scene_->getGraph()->saveOnnx(path.toStdString(), overwrite);
  if (!ret) {
    QMessageBox::critical(this, tr("Save Error"), tr("save failed"),
                          QMessageBox::Ok);
    return false;
  }
  return true;
}

Scene *View::getScene() {
  CHECK_NOTNULL(scene_);
  return scene_;
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

  QPen pfine(ctx_.ui.view.mBackgroundFineGridColor, 1.0);

  painter->setPen(pfine);
  drawGrid(15);

  QPen p(ctx_.ui.view.mBackgroundCoarseGridColor, 1.0);

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

  QTransform t = transform();

  if (t.m11() < 0.15) return;

  scale(factor, factor);
}

void View::scaleExtremeUp() {
  QTransform t = transform();
  do {
    scaleUp();
  } while (t != transform());
}

void View::scaleExtremeDown() {
  QTransform t = transform();
  do {
    scaleDown();
  } while (t != transform());
}

void View::setScale(qreal scale) {
  QTransform t(scale, 0, 0, scale, 0, 0);
  setTransform(t);
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

void View::expandAbs(qreal w, qreal h) {
  QRectF rectTmp = scene()->sceneRect();
  QPointF ptTopLeft = rectTmp.topLeft();
  QPointF ptBottomRight = rectTmp.bottomRight();
  QPointF ptW_H = QPointF(w, h);
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

void View::centertop() {
  QScrollBar *hBar = horizontalScrollBar();
  QScrollBar *vBar = verticalScrollBar();
  hBar->setValue(hBar->minimum() + (hBar->maximum() - hBar->minimum()) / 2);
  vBar->setValue(vBar->minimum());
  update();
}

void View::focusOn(GraphItemBase *item, bool clean) {
  if (clean) {
    scene_->clearSelection();
  }
  CHECK_NOTNULL(item);
  // now edge is not selectable
  if (item->getItemType() != GraphItemType::kNode || item->getDeleted()) {
    return;
  }
  centerOn(item);
  item->setSelected(true);
  scene_->setFocusItem(item, Qt::MenuBarFocusReason);
}

}  // namespace graph
}  // namespace gui
