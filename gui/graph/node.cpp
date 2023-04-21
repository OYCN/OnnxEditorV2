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

#include "gui/ui/dialog/iosummary/iosummary.h"
#include "gui/ui/dialog/nodesummary/nodesummary.h"

namespace gui {
namespace graph {

Node::Node(Context &ctx) : ctx_(ctx) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  setZValue(2);
}

QRectF Node::boundingRect() const { return border_; }

void Node::bind(NodeHandle handle) {
  handle_ = handle;
  refresh();
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

DimStr Node::getDim() const {
  if (handle_->getAttr("setDim") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    return ionode_handle->getDim();
  } else {
    return DimStr();
  }
}

bool Node::setDim(DimStr dim) {
  if (handle_->getAttr("setDim") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    return ionode_handle->setDim(dim);
  } else {
    return false;
  }
}

QString Node::getDataType() const {
  if (handle_->getAttr("setDim") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    return QString::fromStdString(ionode_handle->getDataType());
  } else {
    return {};
  }
}

bool Node::setDataType(QString type) {
  if (handle_->getAttr("setDataType") == "true") {
    CHECK_EQ(handle_->getAttr("NodeType"), "IONode");
    auto ionode_handle = dynamic_cast<utils::simonnx::IONodeObj *>(handle_);
    return ionode_handle->setDataType(type.toStdString());
  } else {
    return false;
  }
}

QList<QList<QString>> Node::getAttrs() const {
  QList<QList<QString>> ret;
  for (auto attr : handle_->getAttrs()) {
    QList<QString> r;
    r.append(QString::fromStdString(attr.key));
    r.append(QString::fromStdString(attr.type));
    r.append(QString::fromStdString(attr.value));
    CHECK_EQ(r.size(), 3);
    ret.append(r);
  }
  return ret;
}

bool Node::setAttrs(const QList<QList<QString>> &attrs) {
  decltype(handle_->getAttrs()) ret;
  for (auto attr : attrs) {
    ret.emplace_back();
    auto &r = ret.back();
    CHECK_EQ(attr.size(), 3);
    r.key = attr[0].toStdString();
    r.type = attr[1].toStdString();
    r.value = attr[2].toStdString();
  }
  return handle_->setAttrs(ret);
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
  mHovered = true;
  update();
  QGraphicsItem::hoverEnterEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  mHovered = false;
  update();
  QGraphicsItem::hoverLeaveEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  if (handle_->getAttr("NodeType") == "IONode") {
    QString name = getName();
    QString type = getDataType();
    DimStr dim = getDim();
    IOSummary d("io node", &name, &type, &dim, ctx_.top_widget);
    auto ret = d.exec();
    if (ret == QDialog::Accepted) {
      if (!setDataType(type)) {
        QMessageBox::critical(
            ctx_.top_widget, "Warning",
            "DataType is invalid, but other change has been saved.");
      }
      if (!setName(name)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "name set error");
      }
      if (!setDim(dim)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "dim set error");
      }
      refresh();
      ioUpdateSend();
    }
  } else {
    QString name = getName();
    QString op_type = getOpType();
    QList<QString> ins = getInputs();
    QList<QString> outs = getOutputs();
    auto attrs = getAttrs();
    NodeSummary d(&name, &op_type, &ins, &outs, &attrs, ctx_.top_widget);
    auto ret = d.exec();
    if (ret == QDialog::Accepted) {
      if (!setOpType(op_type)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "set op_type error");
      }
      if (!setName(name)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "set op_type error");
        LOG(ERROR) << "set name error";
      }
      if (!setInputs(ins)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "set inputs error");
      }
      if (!setOutputs(outs)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "set outputs error");
      }
      if (!setAttrs(attrs)) {
        QMessageBox::critical(ctx_.top_widget, "Error", "set attrs error");
      }
      refresh();
      ioUpdateSend();
    }
  }
  QGraphicsItem::mouseDoubleClickEvent(event);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseMoveEvent(event);
  if (isSelected()) {
    ctx_.selectedNodePosUpdateSend();
  }
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QGraphicsItem::contextMenuEvent(event);
  // menu_.updateStatus();
  // menu_.exec(event->screenPos());
  event->accept();
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change,
                          const QVariant &value) {
  // if (change == QGraphicsItem::ItemPositionChange) {
  //   LOG(INFO) << "ItemPositionChange";
  //   // ctx_.nodePosUpdateSend(this);
  // } else if (change == QGraphicsItem::ItemPositionHasChanged) {
  //   LOG(INFO) << "ItemPositionHasChanged";
  //   // ctx_.nodePosUpdateSend(this);
  // } else
  if (change == QGraphicsItem::ItemSelectedHasChanged) {
    setFlag(QGraphicsItem::ItemIsMovable,
            value.toBool() && ctx_.display.node.movable);
  }
  return QGraphicsItem::itemChange(change, value);
}

void Node::refresh() {
  display_.clear();
  auto op_type = QString::fromStdString(handle_->getOpType());
  if (ctx_.display.node.hidden_op_type.contains(op_type) || getDeleted()) {
    this->setVisible(false);
    return;
  } else {
    this->setVisible(true);
  }

  bool if_display_op_type =
      ctx_.display.node.op_type &&
      !ctx_.display.node.only_display_name.contains(op_type);
  bool if_display_name = ctx_.display.node.name ||
                         ctx_.display.node.only_display_name.contains(op_type);

  // op_type
  Display_t op_type_display;
  if (if_display_op_type) {
    if (ctx_.display.node.redirect_op_type.contains(op_type)) {
      op_type = ctx_.display.node.redirect_op_type[op_type];
    }
    op_type_display.txt = op_type;
    op_type_display.font = QFont(ctx_.ui.node.mFont);
    op_type_display.font.setBold(true);
    QFontMetrics op_type_fm(op_type_display.font);
    QRectF op_type_rect = op_type_fm.boundingRect(op_type);
    op_type_display.txt_rect =
        QRectF(0, 0, op_type_rect.width(), op_type_rect.height());
  }

  // name
  Display_t name_display;
  if (if_display_name) {
    name_display.txt = QString::fromStdString(handle_->getName());
    name_display.font = QFont(ctx_.ui.node.mFont);
    name_display.font.setBold(false);
    QFontMetrics name_fm(name_display.font);
    name_display.txt = name_fm.elidedText(name_display.txt, Qt::ElideMiddle,
                                          ctx_.ui.node.name_max_width);
    QRectF name_rect = name_fm.boundingRect(name_display.txt);
    name_display.txt_rect = QRectF(0, 0, name_rect.width(), name_rect.height());
  }

  if (if_display_op_type && !if_display_name) {
    setToolTip(QString::fromStdString(handle_->getName()));
  } else if (!if_display_op_type && if_display_name) {
    setToolTip(QString::fromStdString(handle_->getOpType()));
  } else if (!if_display_op_type && !if_display_name) {
    setToolTip(QString::fromStdString(handle_->getOpType()) + "\n" +
               QString::fromStdString(handle_->getName()));
  } else {
    setToolTip("");
  }

  // attrs
  QList<Display_t> attrs_display;
  if (ctx_.display.node.attr) {
    auto attrs = getAttrs();
    for (size_t i = 0; i < attrs.size(); i++) {
      Display_t attr_display;
      auto &attr = attrs[i];
      auto str = attr[0];
      auto type = attr[1];
      auto value = attr[2];
      value.replace("\n", "\\n");

      // limit str & val size
      attr_display.font = QFont(ctx_.ui.node.mFont);
      QFontMetrics attr_fm(attr_display.font);
      int all_size =
          ctx_.ui.node.attr_key_max_width + ctx_.ui.node.attr_val_max_width;
      if (str.size() > value.size()) {
        str = attr_fm.elidedText(str, Qt::ElideMiddle,
                                 ctx_.ui.node.attr_key_max_width);
        QRectF str_rect = attr_fm.boundingRect(str);
        value = attr_fm.elidedText(value, Qt::ElideMiddle,
                                   all_size - str_rect.width());
      } else {
        value = attr_fm.elidedText(value, Qt::ElideMiddle,
                                   ctx_.ui.node.attr_val_max_width);
        QRectF value_rect = attr_fm.boundingRect(value);
        str = attr_fm.elidedText(str, Qt::ElideMiddle,
                                 all_size - value_rect.width());
      }

      if (type.back() == 'S') {
        // is Array
        int i = 0;
        for (auto token :
             value.remove('[').remove(']').remove(' ').split(',')) {
          if (token.size() != 0) {
            i++;
          }
        }
        str += QString(" %1<%2>").arg(type).arg(i);
      } else {
        str += " " + value;
      }
      attr_display.txt = str;
      QRectF attr_rect = attr_fm.boundingRect(attr_display.txt);
      attr_display.txt_rect =
          QRectF(0, 0, attr_rect.width(), attr_rect.height());
      attrs_display.append(attr_display);
    }
  }

  // get max width
  qreal max_w = 0;
  {
    auto aw = op_type_display.txt_rect.width();
    auto bw = name_display.txt_rect.width();
    max_w = aw > bw ? aw : bw;
    for (const auto &attr : attrs_display) {
      if (max_w < attr.txt_rect.width()) {
        max_w = attr.txt_rect.width();
      }
    }
  }

  display_.clear();
  QRectF attrs_border_rect;
  // reset txt_rect and border_rect
  {
    qreal off = ctx_.ui.node.mPadTitleT;
    if (if_display_op_type) {
      op_type_display.txt_rect.translate(ctx_.ui.node.mPadOpTypeL,
                                         off + ctx_.ui.node.mPadOpTypeT);
      off = 0;
      op_type_display.txt_rect.setWidth(max_w);
      op_type_display.border_rect = op_type_display.txt_rect.adjusted(
          -ctx_.ui.node.mPadOpTypeL, -ctx_.ui.node.mPadOpTypeT,
          ctx_.ui.node.mPadOpTypeR, ctx_.ui.node.mPadOpTypeB);
      CHECK_EQ(op_type_display.border_rect.x(), 0);
      op_type_display.flag = Qt::AlignCenter;
      display_.append(op_type_display);
    }
    if (if_display_name) {
      name_display.txt_rect.translate(
          ctx_.ui.node.mPadNameL,
          off + op_type_display.border_rect.bottom() + ctx_.ui.node.mPadNameT);
      name_display.txt_rect.setWidth(max_w);
      name_display.border_rect = name_display.txt_rect.adjusted(
          -ctx_.ui.node.mPadNameL, -ctx_.ui.node.mPadNameT,
          ctx_.ui.node.mPadNameR, ctx_.ui.node.mPadNameB);
      CHECK_EQ(name_display.border_rect.x(), 0);
      name_display.flag = Qt::AlignCenter;
      display_.append(name_display);
    }
    title_.setX(0);
    title_.setY(0);
    title_.setWidth(std::max(op_type_display.border_rect.width(),
                             name_display.border_rect.width()));
    title_.setHeight(op_type_display.border_rect.height() +
                     name_display.border_rect.height() +
                     ctx_.ui.node.mPadTitleT + ctx_.ui.node.mPadTitleB);
    qreal y = title_.bottom() + ctx_.ui.node.mPadAttrsT;
    for (auto &attr : attrs_display) {
      attr.txt_rect.translate(ctx_.ui.node.mPadAttrsL, y);
      attr.txt_rect.setWidth(max_w);
      attr.border_rect = attr.txt_rect.adjusted(-ctx_.ui.node.mPadAttrsL, 0,
                                                ctx_.ui.node.mPadAttrsR, 0);
      CHECK_EQ(attr.border_rect.x(), 0);
      attr.flag = Qt::AlignLeft;
      display_.append(attr);
      attrs_border_rect |= attr.border_rect;
      y = attrs_border_rect.bottom() + ctx_.ui.node.mPadAttrsGap;
    }
    if (attrs_display.size() != 0) {
      attrs_border_rect.adjust(0, -ctx_.ui.node.mPadAttrsT, 0,
                               ctx_.ui.node.mPadAttrsB);
    }
  }

  // set border_rect
  border_ = title_ | attrs_border_rect;
  CHECK_EQ(border_.x(), 0);
  CHECK_EQ(border_.y(), 0);
  title_.setWidth(border_.width());

  update();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *) {
  // display outside border
  {
    painter->save();
    auto color = isSelected() ? ctx_.ui.node.mSelectedBoundaryColor
                              : ctx_.ui.node.mNormalBoundaryColor;
    auto width =
        mHovered ? ctx_.ui.node.mHoveredPenWidth : ctx_.ui.node.mPenWidth;
    QPen p(color, width);
    painter->setPen(p);
    painter->setBrush(ctx_.ui.node.mBgColor);
    painter->drawRoundedRect(border_, 3, 3);
    painter->restore();
  }
  {
    painter->save();
    QPen p(Qt::NoPen);
    painter->setPen(p);
    painter->setBrush(ctx_.ui.node.mTitleColor);
    auto width =
        mHovered ? ctx_.ui.node.mHoveredPenWidth : ctx_.ui.node.mPenWidth;
    width /= 2;
    painter->drawRoundedRect(title_.adjusted(width, width, -width, -width), 3,
                             3);
    painter->restore();
  }

  // display txt
  for (const auto &d : display_) {
    painter->save();
    painter->setFont(d.font);
    painter->setPen(ctx_.ui.node.mFontColor);
    painter->drawText(d.txt_rect, d.flag, d.txt);
    // painter->drawRect(d.txt_rect);
    painter->restore();
  }
}

}  // namespace graph
}  // namespace gui
