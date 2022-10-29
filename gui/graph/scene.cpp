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

// #include "utils/algorithm/external/ogdf/ogdf_proxy.h"

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
  VLOG(1) << "add edge: " << label.toStdString();
  auto e = new Edge(mCfg);
  CHECK_NOTNULL(e);

  if (src != nullptr) {
    CHECK(mNode2Outputs.contains(src));
    mNode2Outputs[src].append(e);
    VLOG(1) << "\t from: " << src->getTitle().toStdString();
  } else {
    VLOG(1) << "\t from: Point(" << srcp.x() << ", " << dstp.y() << ")";
  }

  if (dst != nullptr) {
    CHECK(mNode2Inputs.contains(dst));
    mNode2Inputs[dst].append(e);
    VLOG(1) << "\t from: " << dst->getTitle().toStdString();
  } else {
    VLOG(1) << "\t from: Point(" << dstp.x() << ", " << dstp.y() << ")";
  }

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
  std::vector<std::pair<float, float>> g_whs(g_len);
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
      auto n = e.first == node ? e.second : e.first;
      CHECK_EQ(node2idx.count(n), 1);
      g_inputs[i].push_back(node2idx[n]);
    }
    for (auto output_e : outputs) {
      auto e = mEdge2Nodes[output_e];
      auto n = e.first == node ? e.second : e.first;
      CHECK_EQ(node2idx.count(n), 1);
      g_outputs[i].push_back(node2idx[n]);
    }
  }

  // for Summary
  std::vector<size_t> s_single;
  for (auto node : g_roots) {
    if (g_outputs[node].size() == 0) {
      s_single.push_back(node);
    }
  }

  LOG(INFO) << " === Graph Summary Before Layout === ";
  LOG(INFO) << "\t root num:\t" << g_roots.size();
  LOG(INFO) << "\t node num:\t" << g_len;
  LOG(INFO) << "\t single node num:\t" << s_single.size();

  GraphNode2NodeDescTmp g(g_len, g_outputs, g_inputs, g_whs, g_roots);
  auto ret = Layout::layout(&g, LayoutAlgorithm_t::kOGDF);
  CHECK_EQ(ret.getLen(), idx2node.size());
  for (size_t i = 0; i < ret.getLen(); i++) {
    auto pos = ret.getNodePos(i);
    auto rect = idx2node[i]->boundingRect();
    idx2node[i]->setPos(pos.x - rect.width() / 2, pos.y - rect.height() / 2);
  }
  updateEdgePoints();

  // utils::algorithm::external::ogdf::toSvg(&g, "debugs.svg", true);
}

void Scene::clear() {
  for (const auto e : mEdges) {
    removeItem(e);
    delete e;
  }
  mEdges.clear();
  for (const auto n : mNodes) {
    removeItem(n);
    delete n;
  }
  mNodes.clear();
  update();
}

void Scene::loadGraph(GraphNode2NodeDescExt* g) {
  std::vector<graph::Node*> nodes(g->getLen());
  for (size_t i = 0; i < g->getLen(); i++) {
    QList<QString> attr_key;
    QList<QString> attr_val;
    auto node_handle = g->getNodeHandle(i);
    // for (const auto& kv : g->getNodeAttrs(i)) {
    //   attr_key.append(kv.first.c_str());
    //   attr_val.append(kv.second.c_str());
    // }
    nodes[i] = addNode(node_handle->getOpType().c_str(), attr_key, attr_val);
  }

  size_t edge_count = 0;
  for (size_t i = 0; i < g->getLen(); i++) {
    auto from = nodes[i];
    for (auto j : g->getOutput(i)) {
      auto to = nodes[j];
      auto tensor_handle = g->getTensorHandle(i, j);
      addEdge(from, to, tensor_handle->getName().c_str());
    }
  }
}

}  // namespace graph
}  // namespace gui
