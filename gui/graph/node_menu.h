/**
 * @file node_menu.h
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

#ifndef GUI_GRAPH_NODE_MENU_H_
#define GUI_GRAPH_NODE_MENU_H_

#include <QMenu>

namespace gui {
namespace graph {

class Node;

class NodeMenu : public QMenu {
 public:
  explicit NodeMenu(Node* node);

 public:
  void updateStatus();

 private slots:
  void slot_reset_name();
  void slot_reset_op_type();

 private:
  Node* node_;

  QAction* reset_name_action_;
  QAction* reset_op_type_action_;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_NODE_MENU_H_
