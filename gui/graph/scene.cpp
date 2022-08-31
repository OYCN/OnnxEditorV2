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

#include "gui/graph/edge.h"
#include "gui/graph/node.h"
#include "utils/algorithm/layout.h"

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
  mNode2Inputs[n] = {};
  mNode2Outputs[n] = {};
  return n;
}

Edge* Scene::addEdge(const Node* src, const Node* dst, const QString& label,
              const QPointF& srcp, const QPointF& dstp, bool update) {
  CHECK(mNode2Inputs.contains(dst));
  CHECK(mNode2Outputs.contains(src));
  auto e = new Edge(mCfg);
  CHECK_NOTNULL(e);
  mNode2Inputs[dst].append(e);
  mNode2Outputs[src].append(e);
  CHECK(!mEdge2Nodes.contains(e));
  mEdge2Nodes[e] = {src, dst};
  e->init(src, dst, label, srcp, dstp);
  if (update) {
    e->updatePoints();
  }
  addItem(e);
  mEdges.append(e);
  return e;
}

void Scene::updateEdgePoints() {
  for (auto e : mEdges) {
    e->updatePoints();
  }
}

void Scene::layout() {
  using utils::algorithm::desc::GraphNode2NodeDescTmp;
  using utils::algorithm::layout::Layout;
  using utils::algorithm::layout::LayoutAlgorithm_t;

  std::map<const Node*, size_t> node2idx;
  std::vector<Node*> idx2node(mNodes.size());
  for (size_t i = 0; i < mNodes.size(); i++) {
    auto& node = mNodes[i];
    CHECK_EQ(node2idx.count(node), 0);
    node2idx[node] = i;
    idx2node[i] = node;
  }

  size_t g_len = mNodes.size();
  std::vector<size_t> g_roots;
  std::vector<std::pair<float, float>> g_whs;
  std::vector<std::vector<size_t>> g_outputs(g_len);
  std::vector<std::vector<size_t>> g_inputs(g_len);
  for (size_t i = 0; i < mNodes.size(); i++) {
    auto& node = mNodes[i];
    CHECK(mNode2Inputs.contains(node));
    CHECK(mNode2Outputs.contains(node));
    auto& inputs = mNode2Inputs[node];
    auto& outputs = mNode2Outputs[node];
    if (inputs.size() == 0) {
      g_roots.push_back(i);
    }
    auto rect = node->boundingRect();
    g_whs[i] = {rect.width(), rect.height()};
    for (auto input_e : inputs) {
      auto e = mEdge2Nodes[input_e];
      auto n = e.first == node ?  e.second : e.first;
      CHECK_EQ(node2idx.count(n), 1);
      g_inputs[i].push_back(node2idx[n]);
    }
    for (auto output_e : outputs) {
      auto e = mEdge2Nodes[output_e];
      auto n = e.first == node ?  e.second : e.first;
      CHECK_EQ(node2idx.count(n), 1);
      g_outputs[i].push_back(node2idx[n]);
    }
  }

  GraphNode2NodeDescTmp g(g_len, g_outputs, g_inputs, g_whs, g_roots);
  auto ret = Layout::layout(&g, LayoutAlgorithm_t::kOGDF);
  CHECK_EQ(ret.getLen(), idx2node.size());
  for (size_t i = 0; i < ret.getLen(); i++) {
    auto pos = ret.getNodePos(i);
    idx2node[i]->setPos(pos.x, pos.y);
  }
}

}  // namespace graph
}  // namespace gui
