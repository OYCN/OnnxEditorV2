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

#include "gui/config/ui.h"
#include "utils/algorithm/graph_desc.h"
#include "utils/simonnx/context.h"

namespace gui {
namespace graph {

class Node;
class Edge;

class Scene : public QGraphicsScene {
  Q_OBJECT

 public:
  using GraphNode2NodeDescExt = utils::simonnx::GraphNode2NodeDescExt;

  explicit Scene(config::Ui& cfg, QObject* parent = nullptr);

 public:
  void updateCfg();
  Node* addNode(const QString& title, const QList<QString>& attr_key,
                const QList<QString>& attr_val);
  Edge* addEdge(const Node* src, const Node* dst, const QString& label,
                const QPointF& srcp = {0, 0}, const QPointF& dstp = {0, 0},
                bool update = false);
  void updateEdgePoints();
  void layout();
  void clear();

  void loadGraph(GraphNode2NodeDescExt* g);

 private:
  utils::simonnx::SimOnnxCtx* ctx_ = nullptr;
  config::Ui& mCfg;
  QList<Node*> mNodes;
  QList<Edge*> mEdges;
  QMap<const Node*, QList<Edge*>> mNode2Inputs;
  QMap<const Node*, QList<Edge*>> mNode2Outputs;
  QMap<const Edge*, QPair<const Node*, const Node*>> mEdge2Nodes;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_SCENE_H_
