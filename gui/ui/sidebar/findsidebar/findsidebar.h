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
#ifndef GUI_UI_SIDEBAR_FINDSIDEBAR_FINDSIDEBAR_H_
#define GUI_UI_SIDEBAR_FINDSIDEBAR_FINDSIDEBAR_H_

#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <functional>

#include "gui/graph/node.h"
#include "gui/graph/scene.h"
#include "gui/graph/view.h"

namespace Ui {
class FindSidebar;
}

class FindSidebar : public QWidget {
  Q_OBJECT

 public:
  enum {
    FIND_SIDEBAR_MIN_WIDTH = 360,
  };

 public:
  explicit FindSidebar(gui::graph::Scene* scene, gui::graph::View* view,
                       QWidget* parent = nullptr);

  ~FindSidebar() {}

 public slots:
  void setVisible(bool) override;

  void onItemClickedSlot(QListWidgetItem* item);

  void refresh(const QList<gui::graph::Node*>& nodes);

  void onReturnPressedSlot();

 private:
  void filterNodes(const QString& keywords);

  void fillResultList(const QList<gui::graph::Node*>& nodes);

 private:
  Ui::FindSidebar* ui_ = nullptr;
  QWidget* parent_ = nullptr;
  gui::graph::Scene* scene_ = nullptr;
  gui::graph::View* view_ = nullptr;

 private:
  // show result
  QListWidget* result_list_ = nullptr;
  // filter nodes by search keywords
  QList<gui::graph::Node*>* filter_nodes_ = nullptr;
};

#endif  // GUI_UI_SIDEBAR_FINDSIDEBAR_FINDSIDEBAR_H_
