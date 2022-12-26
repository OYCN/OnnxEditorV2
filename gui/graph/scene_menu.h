/**
 * @file scene_menu.h
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

#ifndef GUI_GRAPH_SCENE_MENU_H_
#define GUI_GRAPH_SCENE_MENU_H_

#include <QMenu>
#include <QPointF>

namespace gui {
namespace graph {

class Scene;

class SceneMenu : public QMenu {
 public:
  explicit SceneMenu(Scene* scene);

 public:
  void setPos(QPointF pos);

 private slots:
  void slot_new_node();

 private:
  Scene* scene_;

  QPointF pos_;

  QAction* new_node_action_;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_NODE_MENU_H_
