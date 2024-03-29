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
#include <QKeyEvent>

#include "gui/graph/edge.h"
#include "gui/graph/node.h"
#include "utils/algorithm/layout.h"

using FakeTensor_t = utils::simonnx::FakeTensor_t;

namespace gui {
namespace graph {

Scene::Scene(Context& ctx, QObject* parent)
    : gui_ctx_(ctx), QGraphicsScene{parent}, menu_(this) {
  // graph_ctx_ = SimOnnxCtx::getSimOnnxCtx();
  graph_ctx_ = SimOnnxCtx::createSimOnnxCtx();
  graph_ctx_->bindby(this);
  connect(&gui_ctx_, &Context::nodeUpdateSignal, this, &Scene::nodeUpdateSlot);
  connect(&gui_ctx_, &Context::selectedNodePosUpdateSignal, this,
          &Scene::selectedNodePosUpdateSlot);
  update();
}

Scene::~Scene() {
  clearSelection();
  releaseGraph();
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
    if (!edges_.contains(v)) {
      addEdge(v);
    }
  }
  for (const auto& v : n->getOutputs()) {
    edge_src_[v].insert(n);
    if (!edges_.contains(v)) {
      addEdge(v);
    }
  }
  update();
  nodeUpdateSlot(n);

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

QPair<Scene::GraphNode2NodeDescTmp, QList<Node*>> Scene::cvt2Desc() {
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

  LOG(INFO) << " === Graph Summary === ";
  LOG(INFO) << "\t root num:\t" << g_roots.size();
  LOG(INFO) << "\t node num:\t" << g_len;
  LOG(INFO) << "\t single node num:\t" << s_single.size();

  return {
      Scene::GraphNode2NodeDescTmp(g_len, g_outputs, g_inputs, g_whs, g_roots),
      idx2node};
}

void Scene::dump(QString path) {
  using utils::algorithm::layout::Layout;
  using utils::algorithm::layout::LayoutAlgorithm_t;
  auto g = cvt2Desc().first;
  LayoutAlgorithm_t algo = LayoutAlgorithm_t::kOGDF;
  auto type_str = gui_ctx_.display.view.layout.toLower();
  if (type_str == "ogdf") {
    algo = LayoutAlgorithm_t::kOGDF;
  } else if (type_str == "graphviz") {
    algo = LayoutAlgorithm_t::kGraphviz;
  } else {
    LOG(ERROR) << "unsupport layout algorithm, fallback to ogdf";
  }
  Layout::dump(&g, algo, path.toStdString());
}

QPair<QRectF, QPointF> Scene::layout() {
  using utils::algorithm::layout::Layout;
  using utils::algorithm::layout::LayoutAlgorithm_t;

  auto dsc = cvt2Desc();
  auto& g = dsc.first;
  auto& idx2node = dsc.second;

  LayoutAlgorithm_t algo = LayoutAlgorithm_t::kOGDF;
  auto type_str = gui_ctx_.display.view.layout.toLower();
  if (type_str == "ogdf") {
    algo = LayoutAlgorithm_t::kOGDF;
  } else if (type_str == "graphviz") {
    algo = LayoutAlgorithm_t::kGraphviz;
  } else {
    LOG(ERROR) << "unsupport layout algorithm, fallback to ogdf";
  }
  auto ret = Layout::layout(&g, algo);
  CHECK_EQ(ret.getLen(), idx2node.size());
  // all graph
  qreal t, b, l, r;
  {
    auto pos = ret.getNodePos(1);
    auto rect = idx2node[1]->boundingRect();
    l = pos.x - rect.width() / 2;
    r = pos.x + rect.width() / 2;
    t = pos.y - rect.height() / 2;
    b = pos.y + rect.height() / 2;
  }
  for (size_t i = 1; i < ret.getLen(); i++) {
    auto pos = ret.getNodePos(i);
    auto rect = idx2node[i]->boundingRect();
    qreal x0 = pos.x - rect.width() / 2;
    qreal x1 = pos.x + rect.width() / 2;
    qreal y0 = pos.y - rect.height() / 2;
    qreal y1 = pos.y + rect.height() / 2;
    if (x0 < l) {
      l = x0;
    }
    if (x1 > r) {
      r = x1;
    }
    if (y0 < t) {
      t = y0;
    }
    if (y1 > b) {
      b = y1;
    }
  }
  // qDebug() << "l: " << l;
  // qDebug() << "r: " << r;
  // qDebug() << "t: " << t;
  // qDebug() << "b: " << b;
  QRectF global_rect(0, 0, r - l, b - t);
  qreal x_off = 0 - l;
  qreal y_off = 0 - t;
  // qDebug() << "x_off: " << x_off;
  // qDebug() << "y_off: " << y_off;
  // the first rank
  QPointF pt(0, 0);
  for (size_t i = 0; i < ret.getLen(); i++) {
    auto pos = ret.getNodePos(i);
    auto rect = idx2node[i]->boundingRect();
    auto x = pos.x - rect.width() / 2 + x_off;
    auto y = pos.y - rect.height() / 2 + y_off;
    if (pt == QPointF(0, 0) && abs(y) < 0.1) {
      pt = QPointF(pos.x + x_off, pos.y + y_off);
    }
    idx2node[i]->setPos(x, y);
    // idx2node[i]->setPos(pos.x, pos.y);
  }
  updateEdge();
  return {global_rect, pt};
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

SimOnnxCtx* Scene::releaseGraph() {
  auto ctx = graph_ctx_;
  graph_ctx_ = nullptr;
  ctx->unbindby(this);
  return ctx;
}

void Scene::loadGraph() {
  CHECK_NOTNULL(graph_ctx_);
  clear();

  auto tensor_handles = graph_ctx_->getObjVec<TensorHandle>();
  for (auto t : tensor_handles) {
    addEdge(t);
  }

  auto node_handles = graph_ctx_->getObjVec<NodeHandle>();
  for (auto n : node_handles) {
    addNode(n);
  }

  // auto create remainder edge
  for (auto e_name : edge_src_.keys()) {
    if (!edges_.contains(e_name)) {
      CHECK_NOTNULL(addEdge(e_name));
    }
  }
  for (auto e_name : edge_dst_.keys()) {
    if (!edges_.contains(e_name)) {
      CHECK_NOTNULL(addEdge(e_name));
    }
  }
}

void Scene::nodeUpdateSlot(Node* node) {
  CHECK_NOTNULL(node);
  node->refresh();
  QSet<QString> ins;
  QSet<QString> outs;
  if (node->isVisible()) {
    auto ins_t = node->getInputs();
    auto outs_t = node->getOutputs();
    ins = QSet<QString>(ins_t.begin(), ins_t.end());
    outs = QSet<QString>(outs_t.begin(), outs_t.end());
  }
  for (auto& in : ins) {
    if (!edges_.contains(in)) {
      auto e = addEdge(in);
      CHECK_NOTNULL(e);
    }
    if (!node_inputs_[node].contains(in)) {
      edge_dst_[in].insert(node);
      updateEdge(in);
    }
  }
  for (auto& out : outs) {
    if (!edges_.contains(out)) {
      auto e = addEdge(out);
      CHECK_NOTNULL(e);
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

void Scene::selectedNodePosUpdateSlot() {
  for (auto _item : selectedItems()) {
    auto item = dynamic_cast<GraphItemBase*>(_item);
    if (item->getItemType() == GraphItemType::kNode) {
      auto node = dynamic_cast<Node*>(item);
      auto ins = node->getInputs();
      auto outs = node->getOutputs();
      for (auto& in : ins) {
        updateEdge(in);
      }
      for (auto& out : outs) {
        updateEdge(out);
      }
    }
  }
  // same as nodeUpdateSlot
  emit sceneRectChanged(sceneRect());
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
  QGraphicsScene::contextMenuEvent(event);
  if (event->isAccepted()) {
    return;
  }
  menu_.setPos(event->scenePos());
  menu_.exec(event->screenPos());
  event->accept();
}

void Scene::keyPressEvent(QKeyEvent* event) {
  QGraphicsScene::keyPressEvent(event);
  if (event->isAccepted()) {
    return;
  }
  switch (event->key()) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace: {
      auto items = selectedItems();
      for (auto& _item : items) {
        auto item = dynamic_cast<GraphItemBase*>(_item);
        if (item->getItemType() == GraphItemType::kNode) {
          item->setDeleted(true);
        }
      }
      break;
    }
    default:
      break;
  }
}

}  // namespace graph
}  // namespace gui
