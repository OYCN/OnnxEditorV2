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

#include <QGraphicsSceneContextMenuEvent>

#include "gui/graph/edge.h"
#include "gui/graph/node.h"
#include "utils/algorithm/layout.h"

using FakeTensor_t = utils::simonnx::FakeTensor_t;

namespace gui {
namespace graph {

Scene::Scene(Context& ctx, QObject* parent)
    : gui_ctx_(ctx), QGraphicsScene{parent}, menu_(this) {
  graph_ctx_ = SimOnnxCtx::getSimOnnxCtx();
  connect(&gui_ctx_, &Context::nodeUpdateSignal, this, &Scene::nodeUpdateSlot);
  update();
}

Node* Scene::addNode(NodeHandle handle) {
  auto n = new Node(gui_ctx_);
  CHECK_NOTNULL(n);
  n->bind(handle);
  addItem(n);
  nodes_.append(n);

  auto inputs = n->getInputs();
  auto outputs = n->getOutputs();
  node_inputs_[n] = QSet<QString>(inputs.begin(), inputs.end());
  node_outputs_[n] = QSet<QString>(outputs.begin(), outputs.end());
  for (const auto& v : n->getInputs()) {
    edge_dst_[v].insert(n);
  }
  for (const auto& v : n->getOutputs()) {
    edge_src_[v].insert(n);
  }
  update();

  return n;
}

Edge* Scene::addEdge(TensorHandle handle) {
  VLOG(1) << "add edge: " << handle->getName();
  QString name = QString::fromStdString(handle->getName());
  if (!edges_.contains(name)) {
    auto e = new Edge(gui_ctx_);
    CHECK_NOTNULL(e);
    e->bind(handle);
    addItem(e);
    edges_[name] = e;
    return e;
  } else {
    VLOG(1) << "already exist, skip";
    return nullptr;
  }
}

Edge* Scene::addEdge(const QString& name) {
  if (edges_.contains(name)) {
    return nullptr;
  }
  FakeTensor_t args = {name.toStdString()};
  auto e_handle = graph_ctx_->CreateTensorObj(args);
  CHECK_NOTNULL(e_handle);
  auto e = addEdge(e_handle);
  if (e == nullptr) {
    graph_ctx_->DeleteObj(e_handle);
    LOG(FATAL) << "wht happened?";
  }
  return e;
}

void Scene::updateEdge() {
  for (const auto& e : edges_) {
    updateEdge(e->getName());
  }
}

void Scene::updateEdge(const QString& name) {
  CHECK(edges_.contains(name)) << name.toStdString();
  if (!edge_dst_.contains(name) && !edge_src_.contains(name)) {
    LOG(INFO) << "Skip tensor which not used: " << name.toStdString();
    return;
  }
  QList<QPointF> src;
  VLOG(1) << "src size: " << name.toStdString() << " -> "
          << edge_src_[name].size();
  VLOG(1) << "dst size: " << name.toStdString() << " -> "
          << edge_dst_[name].size();
  for (const auto& n : edge_src_[name]) {
    if (!n->isVisible()) {
      continue;
    }
    QPointF pos = n->pos();
    QRectF rect = n->boundingRect();
    src.push_back({pos.x() + rect.width() / 2, pos.y() + rect.height()});
  }
  QList<QPointF> dst;
  for (const auto& n : edge_dst_[name]) {
    if (!n->isVisible()) {
      continue;
    }
    QPointF pos = n->pos();
    QRectF rect = n->boundingRect();
    dst.push_back({pos.x() + rect.width() / 2, pos.y()});
  }
  edges_[name]->updateEdge(src, dst);
  update();
}

void Scene::layout() {
  using utils::algorithm::desc::GraphNode2NodeDescTmp;
  using utils::algorithm::layout::Layout;
  using utils::algorithm::layout::LayoutAlgorithm_t;

  std::map<const Node*, size_t> node2idx;
  QList<Node*> idx2node;
  for (auto node : nodes_) {
    if (!node->isVisible()) {
      continue;
    }
    CHECK_EQ(node2idx.count(node), 0);
    node2idx[node] = idx2node.size();
    idx2node.push_back(node);
  }

  size_t g_len = idx2node.size();
  std::vector<size_t> g_roots;
  std::vector<std::pair<float, float>> g_whs(g_len);
  std::vector<std::vector<size_t>> g_outputs(g_len);
  std::vector<std::vector<size_t>> g_inputs(g_len);
  for (size_t i = 0; i < idx2node.size(); i++) {
    auto& node = idx2node[i];
    auto inputs = node->getInputs();
    auto outputs = node->getOutputs();
    if (inputs.size() == 0) {
      g_roots.push_back(i);
    }
    auto rect = node->boundingRect();
    g_whs[i] = {rect.width(), rect.height()};
    for (auto v : inputs) {
      auto src = edge_src_[v];
      for (const auto& s : src) {
        if (node2idx.count(s), 1) {
          g_inputs[i].push_back(node2idx[s]);
        }
      }
    }
    for (auto v : outputs) {
      auto dst = edge_dst_[v];
      for (const auto& d : dst) {
        if (node2idx.count(d), 1) {
          g_outputs[i].push_back(node2idx[d]);
        }
      }
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
  updateEdge();

  // utils::algorithm::external::ogdf::toSvg(&g, "debugs.svg", true);
}

void Scene::clear() {
  for (const auto e : edges_) {
    removeItem(e);
    delete e;
  }
  edges_.clear();
  for (const auto n : nodes_) {
    removeItem(n);
    delete n;
  }
  nodes_.clear();
  edge_src_.clear();
  edge_dst_.clear();
  node_inputs_.clear();
  node_outputs_.clear();
  update();
}

void Scene::loadGraph(SimOnnxCtx* ctx) {
  clear();
  graph_ctx_ = ctx;
  LOG(INFO) << "set graph_ctx_ to " << graph_ctx_;

  auto node_handles = ctx->getObjVec<NodeHandle>();
  for (auto n : node_handles) {
    addNode(n);
  }

  auto tensor_handles = ctx->getObjVec<TensorHandle>();
  for (auto t : tensor_handles) {
    addEdge(t);
  }

  // auto create remainder edge
  for (auto e_name : edge_src_.keys()) {
    if (!edges_.contains(e_name)) {
      CHECK_NOTNULL(addEdge(e_name));
    }
  }
}

void Scene::nodeUpdateSlot(Node* node) {
  CHECK_NOTNULL(node);
  auto ins_t = node->getInputs();
  auto outs_t = node->getOutputs();
  auto ins = QSet<QString>(ins_t.begin(), ins_t.end());
  auto outs = QSet<QString>(outs_t.begin(), outs_t.end());
  for (auto& in : ins) {
    if (!edges_.contains(in)) {
      addEdge(in);
    }
    if (!node_inputs_[node].contains(in)) {
      edge_dst_[in].insert(node);
      updateEdge(in);
    }
  }
  for (auto& out : outs) {
    if (!edges_.contains(out)) {
      addEdge(out);
    }
    if (!node_outputs_[node].contains(out)) {
      edge_src_[out].insert(node);
      updateEdge(out);
    }
  }
  for (auto& in : node_inputs_[node]) {
    if (!ins.contains(in)) {
      edge_dst_[in].remove(node);
      updateEdge(in);
    }
  }
  for (auto& out : node_outputs_[node]) {
    if (!outs.contains(out)) {
      edge_src_[out].remove(node);
      updateEdge(out);
    }
  }
  node_inputs_[node] = ins;
  node_outputs_[node] = outs;

  // without this emit, edge will be clear by other edge paint
  // I don't know why
  emit sceneRectChanged(sceneRect());
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
  QGraphicsScene::contextMenuEvent(event);
  menu_.setPos(event->scenePos());
  menu_.exec(event->screenPos());
}

}  // namespace graph
}  // namespace gui
