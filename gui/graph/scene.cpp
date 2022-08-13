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

#include <glog/logging.h>

#include "gui/graph/node.hpp"
namespace gui {
namespace graph {

Scene::Scene(config::Ui& cfg, QObject* parent)
    : mCfg(cfg), QGraphicsScene{parent} {}

void Scene::updateCfg() {}

Node* Scene::addNode(const QString& title, const QList<QString>& attr_key,
                     const QList<QString>& attr_val) {
  auto n = new Node(mCfg);
  CHECK_NOTNULL(n);
  n->init(title, attr_key, attr_val);
  addItem(n);
  mNodes.append(n);
  return n;
}

}  // namespace graph
}  // namespace gui
