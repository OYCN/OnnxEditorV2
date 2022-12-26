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

#include <QInputDialog>
#include <QMessageBox>

#include "gui/graph/node.h"
#include "gui/graph/scene.h"
#include "gui/ui/dialog/txtsetdialog/txtsetdialog.h"

namespace gui {
namespace graph {

SceneMenu::SceneMenu(Scene *scene) : scene_(scene) {
  new_node_action_ = this->addAction("new node");
  connect(new_node_action_, &QAction::triggered, this,
          &SceneMenu::slot_new_node);
}

void SceneMenu::setPos(QPointF pos) { pos_ = pos; }

void SceneMenu::slot_new_node() {
  QString text;
  TxtSetDialog d("op_type:", text, scene_->gui_ctx_.top_widget);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    if (text.isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("name is empty"));
    } else {
      auto handle = scene_->graph_ctx_->CreateNewNodeObj();
      CHECK_NOTNULL(handle);
      auto n = scene_->addNode(handle);
      n->setPos(pos_);
      n->setOpType(text);
      n->setName("debug test");
      n->refresh();
      n->ioUpdateSend();
    }
  }
}

}  // namespace graph
}  // namespace gui
