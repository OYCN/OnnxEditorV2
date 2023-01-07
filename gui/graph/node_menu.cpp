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
  reset_name_action_ = this->addAction("edit name");
  reset_op_type_action_ = this->addAction("edit op_type");
  reset_inputs_action_ = this->addAction("edit inputs");
  reset_outputs_action_ = this->addAction("edit outputs");
  reset_dim_action_ = this->addAction("edit dim");
  connect(reset_name_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_name);
  connect(reset_op_type_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_op_type);
  connect(reset_inputs_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_inputs);
  connect(reset_outputs_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_outputs);
  connect(reset_dim_action_, &QAction::triggered, this,
          &NodeMenu::slot_reset_dim);
}

void NodeMenu::updateStatus() {
  auto if_enable = [&](std::string field) {
    return node_->handle_->getAttr(field, "false") == "true";
  };
  reset_name_action_->setEnabled(if_enable("setName"));
  reset_op_type_action_->setEnabled(if_enable("setOpType"));
  reset_inputs_action_->setEnabled(if_enable("setInputs"));
  reset_outputs_action_->setEnabled(if_enable("setOutputs"));
  reset_dim_action_->setEnabled(if_enable("setDim"));
}

void NodeMenu::slot_reset_name() {
  QString text = node_->getName();
  TxtSetDialog d("name:", &text, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (text.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("name is empty"));
    } else if (node_->setName(text)) {
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set name error"));
    }
  }
}

void NodeMenu::slot_reset_op_type() {
  QString text = node_->getOpType();
  TxtSetDialog d("op_type:", &text, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (text.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("op_type is empty"));
    } else if (node_->setOpType(text)) {
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set op_type error"));
    }
  }
}

void NodeMenu::slot_reset_inputs() {
  QList<QString> out = node_->getInputs();
  TxtListSetDialog d("inputs:", &out, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    node_->setInputs(out);
    node_->ioUpdateSend();
  }
}

void NodeMenu::slot_reset_outputs() {
  QList<QString> out = node_->getOutputs();
  TxtListSetDialog d("outputs:", &out, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    node_->setOutputs(out);
    node_->ioUpdateSend();
  }
}

void NodeMenu::slot_reset_dim() {
  auto dim = node_->getDim();
  TxtSetDialog d("dim:", &dim, node_->ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (node_->setDim(dim)) {
      node_->refresh();
    } else {
      QMessageBox::critical(this, tr("Error"), tr("set op_type error"));
    }
  }
}

}  // namespace graph
}  // namespace gui
