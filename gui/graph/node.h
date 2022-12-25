/**
 * @file node.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-19 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_GRAPH_NODE_H_
#define GUI_GRAPH_NODE_H_

#include <QGraphicsItem>
#include <QObject>
#include <QSet>

#include "gui/graph/context.h"
#include "gui/graph/node_menu.h"
#include "utils/simonnx/context.h"

namespace gui {
namespace graph {

using NodeHandle = utils::simonnx::NodeHandle;

class Node : public QGraphicsItem {
  friend class NodeMenu;

 public:
  struct NodeAttr_t {
    QString key;
    QString value;
    QRectF rect_key;
    QRectF rect_val;
  };

 public:
  explicit Node(Context &cfg);
  void bind(NodeHandle handle);
  void refresh();
  void ioUpdateSend();
  QList<QString> getInputs() const;
  bool getInputs(QList<QString> inputs);
  QList<QString> getOutputs() const;
  bool getOutputs(QList<QString> outputs);

 public:
  QRectF boundingRect() const override;
  const QString &getTitle() const { return mTitle; }

 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = 0) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

 private:
  Context &ctx_;
  NodeMenu menu_;

  NodeHandle handle_;

  QString mTitle = "";
  QList<NodeAttr_t> mAttrs;

  QRectF mAllRect;
  QRectF mTitleRect;
  QRectF mFirstAttrRect;
  bool mHovered = false;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_NODE_H_
