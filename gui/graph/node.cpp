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
#include <QMessageBox>
#include <QtGui/QPainter>

#include "gui/ui/dialog/nodesummary/nodesummary.h"

namespace gui {
namespace graph {

Node::Node(Context &ctx) : ctx_(ctx), menu_(this) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  setZValue(2);
}

QRectF Node::boundingRect() const { return mAllRect; }

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *) {
  painter->save();

  // display outside border
  {
    auto color = isSelected() ? ctx_.ui.node.mSelectedBoundaryColor
                              : ctx_.ui.node.mNormalBoundaryColor;
    auto width =
        mHovered ? ctx_.ui.node.mHoveredPenWidth : ctx_.ui.node.mPenWidth;
    QPen p(color, width);
    painter->setPen(p);
    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, mAllRect.height()));
    gradient.setColorAt(0.0, ctx_.ui.node.mGradientColor0);
    gradient.setColorAt(0.03, ctx_.ui.node.mGradientColor1);
    gradient.setColorAt(0.97, ctx_.ui.node.mGradientColor2);
    gradient.setColorAt(1.0, ctx_.ui.node.mGradientColor3);
    painter->setBrush(gradient);
    painter->drawRoundedRect(mAllRect, 3, 3);
  }

  // display title
  {
    QFont f(ctx_.ui.node.mFont);
    f.setBold(true);
    painter->setFont(f);
    painter->setPen(ctx_.ui.node.mFontColor);
    painter->drawText(mTitleRect, Qt::AlignCenter, mTitle);
  }

  // display attrs
  {
    QFont f(ctx_.ui.node.mFont);
    painter->setFont(f);
    painter->setPen(ctx_.ui.node.mFontColor);
    for (const auto &attr : mAttrs) {
      painter->drawText(attr.rect_key, Qt::AlignLeft, attr.key);
      painter->drawText(attr.rect_val, Qt::AlignLeft, attr.value);
    }
  }

  painter->restore();
}

void Node::bind(NodeHandle handle) {
  handle_ = handle;
  refresh();
}

void Node::refresh() {
  auto op_type = QString::fromStdString(handle_->getOpType());
  auto name = QString::fromStdString(handle_->getName());
  if (ctx_.display.node.hidden_op_type.contains(op_type) || getDeleted()) {
    this->setVisible(false);
  } else {
    this->setVisible(true);
  }

  mTitle = "";
  if (ctx_.display.node.name) {
    mTitle += name;
  }
  if (ctx_.display.node.op_type) {
    if (ctx_.display.node.redirect_op_type.contains(op_type)) {
      op_type = ctx_.display.node.redirect_op_type[op_type];
    }
    mTitle = "[" + op_type + "]" + mTitle;
  }

  QList<QString> attr_keys;
  QList<QString> attr_vals;

  // title
  QFont title_font(ctx_.ui.node.mFont);
  title_font.setBold(true);
  QFontMetrics title_fm(title_font);
  QRectF title_rect = title_fm.boundingRect(mTitle);
  auto base_title_rect = QRectF(0, 0, title_rect.width(), title_rect.height());
  title_rect = base_title_rect.translated(
      ctx_.ui.node.mPadOutsideL + ctx_.ui.node.mPadTitleL,
      ctx_.ui.node.mPadOutsideT + ctx_.ui.node.mPadTitleT);
  QRectF title_rect_after_padding =
      title_rect.adjusted(-ctx_.ui.node.mPadTitleL, -ctx_.ui.node.mPadTitleT,
                          ctx_.ui.node.mPadTitleR, ctx_.ui.node.mPadTitleB);

  // attrs
  QFont attrs_font(ctx_.ui.node.mFont);
  QFontMetrics attr_fm(attrs_font);
  // fake a attr to get the rect
  QRectF span_rect = attr_fm.boundingRect(ctx_.ui.node.mSpan);

  // TODO(opluss): add cfg field to adjust style about attrs display format
  // now, we will using left align style

  // #1 search the max width rect about k+v
  auto search_max_len_rect = [&](decltype(attr_keys) &ins) {
    QRectF max_len_rect(
        0, title_rect_after_padding.bottomLeft().y() + ctx_.ui.node.mPadAttrsT,
        0, 0);
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
  QRectF key_rect = max_key_rect.translated(
      ctx_.ui.node.mPadOutsideL + ctx_.ui.node.mPadAttrsL, 0);
  QRectF val_rect =
      key_rect.translated(key_rect.width() + span_rect.width(), 0);
  CHECK_EQ(key_rect.height(), val_rect.height());
  qreal h_offset =
      key_rect.height() + ctx_.ui.node.mPadAttrsB + ctx_.ui.node.mPadAttrsT;
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
    attrs_rect_after_padding.adjust(
        -ctx_.ui.node.mPadAttrsL, -ctx_.ui.node.mPadAttrsT,
        ctx_.ui.node.mPadAttrsR, ctx_.ui.node.mPadAttrsB);
  }

  // all rect which is after padding
  mAllRect = title_rect_after_padding | attrs_rect_after_padding;
  mAllRect.adjust(-ctx_.ui.node.mPadOutsideL, -ctx_.ui.node.mPadOutsideT,
                  ctx_.ui.node.mPadOutsideR, ctx_.ui.node.mPadOutsideB);
  mTitleRect = QRectF(mAllRect.width() / 2 - title_rect.width() / 2,
                      title_rect.y(), title_rect.width(), title_rect.height());
  CHECK_NEAR(mAllRect.x(), 0, 1e-4);
  CHECK_NEAR(mAllRect.y(), 0, 1e-4);
  update();
}

void Node::ioUpdateSend() { ctx_.nodeUpdateSend(this); }

QString Node::getName() { return QString::fromStdString(handle_->getName()); }

bool Node::setName(QString name) {
  if (handle_->setName(name.toStdString())) {
    if (name != getName()) {
      LOG(FATAL) << "internal error";
    }
    return true;
  } else {
    return false;
  }
}

QString Node::getOpType() {
  return QString::fromStdString(handle_->getOpType());
}

bool Node::setOpType(QString op_type) {
  if (handle_->setOpType(op_type.toStdString())) {
    if (op_type != getOpType()) {
      LOG(FATAL) << "internal error";
    }
    return true;
  } else {
    return false;
  }
}

QList<QString> Node::getInputs() const {
  QList<QString> ret;
  for (const auto &n : handle_->getInputs()) {
    ret.append(QString::fromStdString(n));
  }
  return ret;
}

bool Node::setInputs(QList<QString> inputs) {
  std::vector<std::string> ins(inputs.size());
  for (int i = 0; i < inputs.size(); i++) {
    ins[i] = inputs[i].toStdString();
  }
  return handle_->setInputs(ins);
}

QList<QString> Node::getOutputs() const {
  QList<QString> ret;
  for (const auto &n : handle_->getOutputs()) {
    ret.append(QString::fromStdString(n));
  }
  return ret;
}

bool Node::setOutputs(QList<QString> outputs) {
  std::vector<std::string> outs(outputs.size());
  for (int i = 0; i < outputs.size(); i++) {
    outs[i] = outputs[i].toStdString();
  }
  return handle_->setOutputs(outs);
}

QList<int64_t> Node::getDim() const {
  if (handle_->getAttr("setDim") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    auto dim = ionode_handle->getDim();
    return QList(dim.begin(), dim.end());
  } else {
    return {};
  }
}

bool Node::setDim(QList<int64_t> dim) {
  if (handle_->getAttr("setDim") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    return ionode_handle->setDim({dim.begin(), dim.end()});
  } else {
    return false;
  }
}

void Node::setDeleted(bool del) {
  if (del != getDeleted()) {
    if (del) {
      handle_->getCtx()->DeleteObj(handle_);
    } else {
      handle_->getCtx()->RestoreObj(handle_);
    }
    ioUpdateSend();
  }
}

bool Node::getDeleted() const { return handle_->isDeleted(); }

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
  QString name = getName();
  QString op_type = getOpType();
  QList<QString> ins = getInputs();
  QList<QString> outs = getOutputs();
  NodeSummary d(name, op_type, ins, outs, ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (name.isEmpty()) {
      QMessageBox::critical(ctx_.top_widget, "Error", "name is empty");
    } else if (op_type.isEmpty()) {
      QMessageBox::critical(ctx_.top_widget, "Error", "op_type is empty");
    } else {
      setOpType(op_type);
      setName(name);
      setInputs(ins);
      setOutputs(outs);
      refresh();
      ioUpdateSend();
    }
  }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // qDebug() << "Double press: " << this;
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QGraphicsItem::contextMenuEvent(event);
  menu_.updateStatus();
  menu_.exec(event->screenPos());
  event->accept();
}

}  // namespace graph
}  // namespace gui
