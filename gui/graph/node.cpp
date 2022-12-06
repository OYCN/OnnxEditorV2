/**
 * @file node.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-19 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/node.h"

#include <glog/logging.h>

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QtGui/QPainter>

namespace gui {
namespace graph {

Node::Node(config::Ui &cfg) : mCfg(cfg), menu_(this) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
}

QRectF Node::boundingRect() const { return mAllRect; }

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *) {
  painter->save();

  // display outside border
  {
    auto color = isSelected() ? mCfg.node.mSelectedBoundarColor
                              : mCfg.node.mNormalBoundarColor;
    auto width = mHovered ? mCfg.node.mHoveredPenWidth : mCfg.node.mPenWidth;
    QPen p(color, width);
    painter->setPen(p);
    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, mAllRect.height()));
    gradient.setColorAt(0.0, mCfg.node.mGradientColor0);
    gradient.setColorAt(0.03, mCfg.node.mGradientColor1);
    gradient.setColorAt(0.97, mCfg.node.mGradientColor2);
    gradient.setColorAt(1.0, mCfg.node.mGradientColor3);
    painter->setBrush(gradient);
    painter->drawRoundedRect(mAllRect, 3, 3);
  }

  // display title
  {
    QFont f(mCfg.node.mFont);
    f.setBold(true);
    painter->setFont(f);
    painter->setPen(mCfg.node.mFontColor);
    painter->drawText(mTitleRect, Qt::AlignCenter, mTitle);
  }

  // display attrs
  {
    QFont f(mCfg.node.mFont);
    painter->setFont(f);
    painter->setPen(mCfg.node.mFontColor);
    for (const auto &attr : mAttrs) {
      painter->drawText(attr.rect_key, Qt::AlignLeft, attr.key);
      painter->drawText(attr.rect_val, Qt::AlignLeft, attr.value);
    }
  }

  painter->restore();
}

void Node::init(NodeHandle handle) {
  handle_ = handle;
  // mTitle = handle_->getName().c_str();
  mTitle = handle_->getOpType().c_str();
  QList<QString> attr_keys;
  QList<QString> attr_vals;

  // title
  QFont title_font(mCfg.node.mFont);
  title_font.setBold(true);
  QFontMetrics title_fm(title_font);
  QRectF title_rect = title_fm.boundingRect(mTitle);
  auto base_title_rect = QRectF(0, 0, title_rect.width(), title_rect.height());
  title_rect =
      base_title_rect.translated(mCfg.node.mPadOutsideL + mCfg.node.mPadTitleL,
                                 mCfg.node.mPadOutsideT + mCfg.node.mPadTitleT);
  QRectF title_rect_after_padding =
      title_rect.adjusted(-mCfg.node.mPadTitleL, -mCfg.node.mPadTitleT,
                          mCfg.node.mPadTitleR, mCfg.node.mPadTitleB);

  // attrs
  QFont attrs_font(mCfg.node.mFont);
  QFontMetrics attr_fm(attrs_font);
  // fake a attr to get the rect
  QRectF span_rect = attr_fm.boundingRect(mCfg.node.mSpan);

  // TODO(opluss): add cfg field to adjust style about attrs display format
  // now, we will using left align style

  // #1 search the max width rect about k+v
  auto search_max_len_rect = [&](decltype(attr_keys) &ins) {
    QRectF max_len_rect(
        0, title_rect_after_padding.bottomLeft().y() + mCfg.node.mPadAttrsT, 0,
        0);
    for (const auto &i : ins) {
      auto this_rect = attr_fm.boundingRect(i);
      if (max_len_rect.width() < this_rect.width()) {
        max_len_rect.setWidth(this_rect.width());
        max_len_rect.setHeight(this_rect.height());
      }
    }
    return max_len_rect;
  };
  auto max_key_rect = search_max_len_rect(attr_keys);
  auto max_val_rect = search_max_len_rect(attr_vals);
  // qDebug() << "max_key_rect:" << max_key_rect;
  // qDebug() << "max_val_rect:" << max_val_rect;

  // #2 pad the key/val and caculate the rect
  QRectF key_rect =
      max_key_rect.translated(mCfg.node.mPadOutsideL + mCfg.node.mPadAttrsL, 0);
  QRectF val_rect =
      key_rect.translated(key_rect.width() + span_rect.width(), 0);
  CHECK_EQ(key_rect.height(), val_rect.height());
  qreal h_offset =
      key_rect.height() + mCfg.node.mPadAttrsB + mCfg.node.mPadAttrsT;
  mAttrs.clear();
  // qDebug() << "key_rect:" << key_rect;
  // qDebug() << "val_rect:" << val_rect;
  // qDebug() << "h_offset:" << h_offset;
  for (size_t i = 0; i < attr_keys.size(); i++) {
    NodeAttr_t nad;
    nad.key = attr_keys[i];
    nad.value = attr_vals[i];
    nad.rect_key = key_rect;
    nad.rect_val = val_rect;
    key_rect.translate(0, h_offset);
    val_rect.translate(0, h_offset);
    mAttrs.emplace_back(nad);
  }

  QRectF attrs_rect_after_padding(0, 0, 0, 0);
  if (!mAttrs.empty()) {
    attrs_rect_after_padding = mAttrs[0].rect_key | mAttrs.back().rect_val;
    attrs_rect_after_padding.adjust(-mCfg.node.mPadAttrsL,
                                    -mCfg.node.mPadAttrsT, mCfg.node.mPadAttrsR,
                                    mCfg.node.mPadAttrsB);
  }

  // all rect which is after padding
  mAllRect = title_rect_after_padding | attrs_rect_after_padding;
  mAllRect.adjust(-mCfg.node.mPadOutsideL, -mCfg.node.mPadOutsideT,
                  mCfg.node.mPadOutsideR, mCfg.node.mPadOutsideB);
  mTitleRect = QRectF(mAllRect.width() / 2 - title_rect.width() / 2,
                      title_rect.y(), title_rect.width(), title_rect.height());
  CHECK_NEAR(mAllRect.x(), 0, 1e-4);
  CHECK_NEAR(mAllRect.y(), 0, 1e-4);
  update();
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  // qDebug() << "Enter hover";
  mHovered = true;
  update();
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  // qDebug() << "Leave hover";
  mHovered = false;
  update();
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  // qDebug() << "Double click: " << this;
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // qDebug() << "Double press: " << this;
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  menu_.exec(event->screenPos());
}

}  // namespace graph
}  // namespace gui
