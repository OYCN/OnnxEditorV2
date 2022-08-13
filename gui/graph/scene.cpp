/**
 * @file scene.cpp
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

#include "gui/graph/scene.h"

namespace gui {
namespace graph {

Scene::Scene(config::Ui& cfg, QObject* parent)
    : mCfg(cfg), QGraphicsScene{parent} {}

void Scene::updateCfg() {}

}  // namespace graph
}  // namespace gui
