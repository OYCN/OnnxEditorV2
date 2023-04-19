/**
 * @file findsidebar.cpp
 * @brief
 * @author beinggod (beinggod@foxmail.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-04-17 <td>1.0     <td>beinggod    <td>Initial
 * </table>
 */
#include "gui/ui/sidebar/findsidebar/findsidebar.h"

#include <QLineEdit>
#include <QList>
#include <QWidget>
#include <string>

#include "glog/logging.h"
#include "gui/graph/node.h"
#include "gui/ui/sidebar/findsidebar/ui_findsidebar.h"

FindSidebar::FindSidebar(gui::graph::Scene* scene, gui::graph::View* view,
                         QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::FindSidebar),
      view_(view),
      scene_(scene),
      parent_(parent) {
  ui_->setupUi(this);

  result_list_ = ui_->result_list_widget;
  filter_nodes_ = new QList<gui::graph::Node*>();

  connect(result_list_, &QListWidget::itemClicked, this,
          &FindSidebar::onItemClickedSlot);

  connect(ui_->search_edit, &QLineEdit::returnPressed, this,
          &FindSidebar::onReturnPressedSlot);

  // default not visible
  setVisible(false);
}

void FindSidebar::onItemClickedSlot(QListWidgetItem* item) {
  CHECK_NOTNULL(item);

  const QString name = item->text();

  // TODO(beinggod): iter current node, due to QListWidgetItem not support
  // downcasting
  for (auto node : scene_->getNodes()) {
    if (node->getName() == name) {
      VLOG(1) << "Selected node name: " << node->getName().toStdString();
      view_->focusOn(node, true);
      return;
    }
  }
}

void FindSidebar::setVisible(bool visible) {
  if (visible == false) {
    ui_->search_edit->clear();
    result_list_->clear();

    QWidget::setVisible(false);
    return;
  }

  int width = FIND_SIDEBAR_MIN_WIDTH;

  setFixedWidth(width);

  refresh(scene_->getNodes());

  QWidget::setVisible(true);
}

void FindSidebar::refresh(const QList<gui::graph::Node*>& nodes) {
  result_list_->clear();

  fillResultList(nodes);
}

void FindSidebar::onReturnPressedSlot() {
  const QString& keywords = ui_->search_edit->text();

  VLOG(1) << "Search keywords:" << keywords.toStdString();

  filterNodes(keywords);

  refresh(*filter_nodes_);
}

void FindSidebar::filterNodes(const QString& keywords) {
  filter_nodes_->clear();

  const std::string& keywords_ = keywords.toLower().toStdString();

  for (auto node : scene_->getNodes()) {
    if (!node->isActive() || node->getDeleted()) {
      continue;
    }

    const std::string& node_name = node->getName().toLower().toStdString();
    const std::string& op_type = node->getOpType().toLower().toStdString();

    if (node_name.find(keywords_) != std::string::npos) {
      // node name
      filter_nodes_->append(node);
    } else if (op_type.find(keywords_) != std::string::npos) {
      // op type
      filter_nodes_->append(node);
    }
  }
}

void FindSidebar::fillResultList(const QList<gui::graph::Node*>& nodes) {
  // TODO(beinggod): use diff to optmize performance
  for (auto node : nodes) {
    if (node->isVisible() && !node->getDeleted()) {
      if (node->getOpType() == "::input::") {
        result_list_->insertItem(0, node->getName());
      } else if (node->getOpType() == "::output::") {
        continue;
      } else {
        result_list_->addItem(node->getName());
      }
    }
  }
}
