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
#include "gui/ui/dialog/txtlistsetdialog/txtlistsetdialog.h"
#include "gui/ui/dialog/txtsetdialog/txtsetdialog.h"

namespace gui {
namespace graph {

NodeMenu::NodeMenu(Node *node) : node_(node) {
  reset_name_action_ = this->addAction("reset name");
  reset_op_type_action_ = this->addAction("reset op_type");
  reset_inputs_action_ = this->addAction("reset inputs");
  reset_outputs_action_ = this->addAction("reset outputs");
  connect(reset_name_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_name);
  connect(reset_op_type_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_op_type);
  connect(reset_inputs_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_inputs);
  connect(reset_outputs_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_outputs);
}

void NodeMenu::updateStatus() {
  auto if_disable = [&](std::string field) {
    return node_->handle_->getAttr(field, "") != "false";
  };
  reset_name_action_->setEnabled(if_disable("setName"));
  reset_op_type_action_->setEnabled(if_disable("setOpType"));
  reset_inputs_action_->setEnabled(if_disable("setInputs"));
  reset_outputs_action_->setEnabled(if_disable("setOutputs"));
}

void NodeMenu::slot_reset_name() {
  QString text = QString::fromStdString(node_->handle_->getName());
  TxtSetDialog d("name:", text, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (text.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("name is empty"));
    } else if (node_->handle_->setName(text.toStdString())) {
      CHECK_EQ(text.toStdString(), node_->handle_->getName());
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set name error"));
    }
  }
}

void NodeMenu::slot_reset_op_type() {
  QString text = QString::fromStdString(node_->handle_->getOpType());
  TxtSetDialog d("op_type:", text, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (text.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("op_type is empty"));
    } else if (node_->handle_->setOpType(text.toStdString())) {
      CHECK_EQ(text.toStdString(), node_->handle_->getOpType());
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set op_type error"));
    }
  }
}

void NodeMenu::slot_reset_inputs() {
  QList<QString> out = node_->getInputs();
  TxtListSetDialog d("inputs:", out, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    node_->getInputs(out);
    node_->ioUpdateSend();
  }
}

void NodeMenu::slot_reset_outputs() {
  QList<QString> out = node_->getOutputs();
  TxtListSetDialog d("outputs:", out, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    node_->getOutputs(out);
    node_->ioUpdateSend();
  }
}

}  // namespace graph
}  // namespace gui
