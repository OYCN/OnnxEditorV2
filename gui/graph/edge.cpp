/**
 * @file edge.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-28 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/edge.h"

#include <glog/logging.h>

#include <QtGui/QPainter>

#include "gui/graph/node.h"

namespace gui {
namespace graph {

Edge::Edge(Context& ctx) : ctx_(ctx) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  setZValue(0);
}

QRectF Edge::boundingRect() const { return shape().boundingRect(); }

QPainterPath Edge::shape() const {
  QPainterPathStroker ps;
  QPen pen;

  if (mHovered || isSelected()) {
    pen.setWidth(2 * ctx_.ui.edge.mEdgeLineWidth);
  } else {
    pen.setWidth(ctx_.ui.edge.mEdgeLineWidth);
  }

  ps.setCapStyle(pen.capStyle());
  ps.setWidth(pen.widthF() + 10);
  ps.setJoinStyle(pen.joinStyle());
  ps.setMiterLimit(pen.miterLimit());

  QPainterPath p = ps.createStroke(mPath);
  // p.addRect(mNameRect);
  for (const auto& s : src_) {
    p.addEllipse(s, ctx_.ui.edge.mEdgeStartPointRadius,
                 ctx_.ui.edge.mEdgeStartPointRadius);
  }
  for (const auto& d : dst_) {
    p.addEllipse(d, ctx_.ui.edge.mEdgeStopPointRadius,
                 ctx_.ui.edge.mEdgeStopPointRadius);
  }
  return p;
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
  painter->save();

  // hovered or selected
  if (mHovered || isSelected()) {
    QPen p;
    p.setWidth(2 * ctx_.ui.edge.mEdgeLineWidth);
    p.setColor(isSelected() ? ctx_.ui.edge.mEdgeSelectedHaloColor
                            : ctx_.ui.edge.mEdgeHoveredColor);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPath);
  }

  // for normal
  {
    QPen p;
    p.setWidth(ctx_.ui.edge.mEdgeLineWidth);
    p.setColor(isSelected() ? ctx_.ui.edge.mEdgeSelectedColor
                            : ctx_.ui.edge.mEdgeNormalColor);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPath);
  }

  // Ellipse
  for (const auto& s : src_) {
    painter->setPen(ctx_.ui.edge.mEdgeStartPointColor);
    painter->setBrush(ctx_.ui.edge.mEdgeStartPointColor);
    painter->drawEllipse(s, ctx_.ui.edge.mEdgeStartPointRadius,
                         ctx_.ui.edge.mEdgeStartPointRadius);
  }
  for (const auto& d : dst_) {
    painter->setPen(ctx_.ui.edge.mEdgeStopPointColor);
    painter->setBrush(ctx_.ui.edge.mEdgeStopPointColor);
    painter->drawEllipse(d, ctx_.ui.edge.mEdgeStopPointRadius,
                         ctx_.ui.edge.mEdgeStopPointRadius);
  }

  // for label
  // {
  //   painter->save();
  //   QFont f(ctx_.ui.edge.mEdgeFont);
  //   painter->setFont(f);
  //   painter->setPen(ctx_.ui.edge.mEdgeFontColor);
  //   painter->drawText(mNameRect, Qt::AlignLeft, mName);
  //   painter->restore();
  // }

  painter->restore();
}

void Edge::bind(TensorHandle handle) { handle_ = handle; }

void Edge::updateEdge(const QList<QPointF>& src, const QList<QPointF>& dst) {
  src_ = src;
  dst_ = dst;

  mPath.clear();
  update();
  if (src_.size() == 0 || dst_.size() == 0) {
    return;
  }
  if (src_.size() > 1) {
    LOG(ERROR) << "Not handled multi src case";
    return;
  }
  auto s = src_[0];
  for (const auto d : dst_) {
    mPath.moveTo(s);
    QPointF c1 = QPointF(s.x(), (s.y() + d.y()) / 2);
    QPointF c2 = QPointF(d.x(), (s.y() + d.y()) / 2);
    mPath.cubicTo(c1, c2, d);
  }

  // update text pos
  // QLineF l(mSrcP, mDstP);
  // auto center = l.center();
  // QFontMetrics fm(QFont(ctx_.ui.edge.mEdgeFont));
  // QRectF rect = fm.boundingRect(mName);
  // mNameRect = QRectF(center.x(), center.y(), rect.width() + 1,
  // rect.height());

  update();
}

QString Edge::getName() const {
  return QString::fromStdString(handle_->getName());
}

void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
  // qDebug() << "Enter hover";
  mHovered = true;
  update();
}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
  // qDebug() << "Leave hover";
  mHovered = false;
  update();
}

}  // namespace graph
}  // namespace gui
