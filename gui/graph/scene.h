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

#include <QGraphicsScene>
#include <QMap>
#include <QSet>

#include "gui/graph/context.h"
#include "gui/graph/edge.h"
#include "gui/graph/node.h"
#include "utils/algorithm/graph_desc.h"
#include "utils/simonnx/context.h"

namespace gui {
namespace graph {

using SimOnnxCtx = utils::simonnx::SimOnnxCtx;

class Scene : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit Scene(Context& ctx, QObject* parent = nullptr);

 public:
  void refreshAll();
  Node* addNode(NodeHandle handle);
  Edge* addEdge(TensorHandle handle);
  Edge* addEdge(const QString& name);
  void updateEdge();
  void updateEdge(const QString& name);
  void layout();
  void clear();

  void loadGraph(SimOnnxCtx* ctx);

 public slots:
  void nodeUpdateSlot(Node* node);

 private:
  SimOnnxCtx* graph_ctx_ = nullptr;
  Context& gui_ctx_;
  QList<Node*> nodes_;
  QMap<QString, Edge*> edges_;
  QMap<QString, QSet<Node*>> edge_src_;
  QMap<QString, QSet<Node*>> edge_dst_;
  QMap<Node*, QSet<QString>> node_inputs_;
  QMap<Node*, QSet<QString>> node_outputs_;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_SCENE_H_
