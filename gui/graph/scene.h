/**
 * @file scene.hpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_GRAPH_SCENE_H_
#define GUI_GRAPH_SCENE_H_

#include <QtWidgets/QGraphicsScene>

#include "gui/config/ui.h"

namespace gui {
namespace graph {

class Scene : public QGraphicsScene {
  Q_OBJECT
 public:
  explicit Scene(config::Ui& cfg, QObject* parent = nullptr);
  void updateCfg();
 private:
  config::Ui& mCfg;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_SCENE_H_
