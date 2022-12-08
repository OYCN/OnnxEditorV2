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

#include <QtGui/QPainter>

#include "gui/graph/node.h"

namespace gui {
namespace graph {

Edge::Edge(config::Ui& cfg) : mCfg(cfg) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  setZValue(0);
}

QRectF Edge::boundingRect() const { return shape().boundingRect(); }

QPainterPath Edge::shape() const {
  QPainterPathStroker ps;
  QPen pen;

  if (mHovered || isSelected()) {
    pen.setWidth(2 * mCfg.edge.mEdgeLineWidth);
  } else {
    pen.setWidth(mCfg.edge.mEdgeLineWidth);
  }

  ps.setCapStyle(pen.capStyle());
  ps.setWidth(pen.widthF() + 10);
  ps.setJoinStyle(pen.joinStyle());
  ps.setMiterLimit(pen.miterLimit());

  QPainterPath p = ps.createStroke(mPath);
  // p.addRect(mLabelRect);
  p.addEllipse(mSrcP, mCfg.edge.mEdgeStartPointRadius,
               mCfg.edge.mEdgeStartPointRadius);
  p.addEllipse(mDstP, mCfg.edge.mEdgeStopPointRadius,
               mCfg.edge.mEdgeStopPointRadius);
  return p;
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
  painter->save();

  // hovered or selected
  if (mHovered || isSelected()) {
    QPen p;
    p.setWidth(2 * mCfg.edge.mEdgeLineWidth);
    p.setColor(isSelected() ? mCfg.edge.mEdgeSelectedHaloColor
                            : mCfg.edge.mEdgeHoveredColor);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPath);
  }

  // for normal
  {
    QPen p;
    p.setWidth(mCfg.edge.mEdgeLineWidth);
    p.setColor(isSelected() ? mCfg.edge.mEdgeSelectedColor
                            : mCfg.edge.mEdgeNormalColor);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPath);
  }

  // Ellipse
  painter->setPen(mCfg.edge.mEdgeStartPointColor);
  painter->setBrush(mCfg.edge.mEdgeStartPointColor);
  painter->drawEllipse(mSrcP, mCfg.edge.mEdgeStartPointRadius,
                       mCfg.edge.mEdgeStartPointRadius);
  painter->setPen(mCfg.edge.mEdgeStopPointColor);
  painter->setBrush(mCfg.edge.mEdgeStopPointColor);
  painter->drawEllipse(mDstP, mCfg.edge.mEdgeStopPointRadius,
                       mCfg.edge.mEdgeStopPointRadius);

  // for label
  // {
  //   painter->save();
  //   QFont f(mCfg.edge.mEdgeFont);
  //   painter->setFont(f);
  //   painter->setPen(mCfg.edge.mEdgeFontColor);
  //   painter->drawText(mLabelRect, Qt::AlignLeft, mLabel);
  //   painter->restore();
  // }

  painter->restore();
}

void Edge::init(const Node* src, const Node* dst, const QString& label,
                const QPointF& srcp, const QPointF& dstp) {
  mLabel = label;
  mSrc = src;
  mDst = dst;

  // we assume the graph is from top to bottom

  // compute the start point which is the top of edge
  if (mSrc != nullptr) {
    QPointF pos = mSrc->pos();
    QRectF rect = mSrc->boundingRect();
    mSrcP = {pos.x() + rect.width() / 2, pos.y() + rect.height()};
  } else {
    mSrcP = srcp;
  }

  // compute the end point which is the bottom of edge
  if (mDst != nullptr) {
    QPointF pos = mDst->pos();
    QRectF rect = mDst->boundingRect();
    mDstP = {pos.x() + rect.width() / 2, pos.y()};
  } else {
    mDstP = dstp;
  }
}

void Edge::updatePoints(const QPointF* srcp, const QPointF* dstp) {
  if (srcp != nullptr) {
    mSrcP = *srcp;
  } else {
    if (mSrc != nullptr) {
      QPointF pos = mSrc->pos();
      QRectF rect = mSrc->boundingRect();
      mSrcP = {pos.x() + rect.width() / 2, pos.y() + rect.height()};
    }
  }

  if (dstp != nullptr) {
    mSrcP = *dstp;
  } else {
    if (mDst != nullptr) {
      QPointF pos = mDst->pos();
      QRectF rect = mDst->boundingRect();
      mDstP = {pos.x() + rect.width() / 2, pos.y()};
    }
  }

  // update path
  mPath.clear();
  mPath.moveTo(mSrcP);

  // QPointF c1 = QPointF((mSrcP.x() + mDstP.x()) / 2, mSrcP.y());
  // QPointF c2 = QPointF((mSrcP.x() + mDstP.x()) / 2, mDstP.y());
  QPointF c1 = QPointF(mSrcP.x(), (mSrcP.y() + mDstP.y()) / 2);
  QPointF c2 = QPointF(mDstP.x(), (mSrcP.y() + mDstP.y()) / 2);
  mPath.cubicTo(c1, c2, mDstP);

  // QPointF c0 = QPointF((mSrcP.x() + mDstP.x()) / 2, mSrcP.y());
  // mPath.quadTo(c0, mDstP);

  // mPath.lineTo(mDstP);

  // update text pos
  QLineF l(mSrcP, mDstP);
  auto center = l.center();

  // QFontMetrics fm(QFont(mCfg.edge.mEdgeFont));
  // QRectF rect = fm.boundingRect(mLabel);
  // mLabelRect = QRectF(center.x(), center.y(), rect.width() + 1, rect.height());

  update();
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
