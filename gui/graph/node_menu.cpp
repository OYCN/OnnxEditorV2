/**
 * @file node_menu.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-06 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/node_menu.h"

#include <glog/logging.h>

#include <QInputDialog>
#include <QMessageBox>

#include "gui/graph/node.h"

namespace gui {
namespace graph {

NodeMenu::NodeMenu(Node *node) : node_(node) {
  reset_name_action_ = this->addAction("reset name");
  reset_op_type_action_ = this->addAction("reset op_yype");
  connect(reset_name_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_name);
  connect(reset_op_type_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_op_type);
}

void NodeMenu::updateStatus() {
  bool state = node_->handle_->getAttr("writable") == "true";
  reset_name_action_->setEnabled(state);
  reset_op_type_action_->setEnabled(state);
}

void NodeMenu::slot_reset_name() {
  bool bRet = false;
  QString text = QInputDialog::getText(
      nullptr, tr("Input"), tr("New name"), QLineEdit::Normal,
      QString::fromStdString(node_->handle_->getName()), &bRet);
  if (bRet && !text.isEmpty()) {
    if (node_->handle_->setName(text.toStdString())) {
      CHECK_EQ(text.toStdString(), node_->handle_->getName());
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set name error"));
    }
  }
}

void NodeMenu::slot_reset_op_type() {
  bool bRet = false;
  QString text = QInputDialog::getText(
      nullptr, tr("Input"), tr("New op_type"), QLineEdit::Normal,
      QString::fromStdString(node_->handle_->getOpType()), &bRet);
  if (bRet && !text.isEmpty()) {
    if (node_->handle_->setOpType(text.toStdString())) {
      CHECK_EQ(text.toStdString(), node_->handle_->getOpType());
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set op_type error"));
    }
  }
}

}  // namespace graph
}  // namespace gui
