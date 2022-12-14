/**
 * @file scene_menu.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-26 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/scene_menu.h"

#include <glog/logging.h>

#include <QMessageBox>

#include "gui/graph/node.h"
#include "gui/graph/scene.h"
#include "gui/ui/dialog/iosummary/iosummary.h"
#include "gui/ui/dialog/nodesummary/nodesummary.h"

namespace gui {
namespace graph {

SceneMenu::SceneMenu(Scene *scene) : scene_(scene) {
  new_node_action_ = this->addAction("new node");
  new_input_action_ = this->addAction("new input");
  new_output_action_ = this->addAction("new output");
  connect(new_node_action_, &QAction::triggered, this,
          &SceneMenu::slot_new_node);
  connect(new_input_action_, &QAction::triggered, this,
          &SceneMenu::slot_new_input);
  connect(new_output_action_, &QAction::triggered, this,
          &SceneMenu::slot_new_output);
}

void SceneMenu::setPos(QPointF pos) { pos_ = pos; }

void SceneMenu::slot_new_node() {
  QString name = "";
  QString op_type = "";
  QList<QString> ins = {};
  QList<QString> outs = {};
  NodeSummary d(&name, &op_type, &ins, &outs, scene_->gui_ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (name.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("name is empty"));
    } else if (op_type.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("op_type is empty"));
    } else {
      auto handle = scene_->graph_ctx_->CreateNewNodeObj();
      CHECK_NOTNULL(handle);
      auto n = scene_->addNode(handle);
      n->setPos(pos_);
      n->setOpType(op_type);
      n->setName(name);
      n->setInputs(ins);
      n->setOutputs(outs);
      n->refresh();
      n->ioUpdateSend();
    }
  }
}

void SceneMenu::slot_new_input() {
  QString name;
  QString type;
  QList<int64_t> dim;
  IOSummary d("input node", &name, &type, &dim, scene_->gui_ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    auto handle = scene_->graph_ctx_->CreateNewIOObj(
        utils::simonnx::IONodeType::kInputNode);
    CHECK_NOTNULL(handle);
    auto n = scene_->addNode(handle);
    n->setPos(pos_);
    n->setName(name);
    n->setDim(dim);
    n->refresh();
    n->ioUpdateSend();
    if (!n->setDataType(type)) {
      QMessageBox::warning(
          this, tr("Warning"),
          tr("DataType is invalid, but node has already created."));
    }
  }
}

void SceneMenu::slot_new_output() {
  QString name;
  QString type;
  QList<int64_t> dim;
  IOSummary d("output node", &name, &type, &dim, scene_->gui_ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    auto handle = scene_->graph_ctx_->CreateNewIOObj(
        utils::simonnx::IONodeType::kOutputNode);
    CHECK_NOTNULL(handle);
    auto n = scene_->addNode(handle);
    n->setPos(pos_);
    n->setName(name);
    n->setDim(dim);
    n->refresh();
    n->ioUpdateSend();
    if (!n->setDataType(type)) {
      QMessageBox::warning(
          this, tr("Warning"),
          tr("DataType is invalid, but node has already created."));
    }
  }
}

}  // namespace graph
}  // namespace gui
