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
#include <QList>
#include <QObject>
#include <QSet>

#include "gui/graph/context.h"
#include "gui/graph/item.h"
#include "gui/graph/node_menu.h"
#include "utils/simonnx/context.h"

namespace gui {
namespace graph {

using NodeHandle = utils::simonnx::NodeHandle;

class Node : public GraphItemBase {
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
  QString getName();
  bool setName(QString name);
  QString getOpType();
  bool setOpType(QString op_type);
  QList<QString> getInputs() const;
  bool setInputs(QList<QString> inputs);
  QList<QString> getOutputs() const;
  bool setOutputs(QList<QString> outputs);
  QList<int64_t> getDim() const;
  bool setDim(QList<int64_t> dim);
  QString getDataType() const;
  bool setDataType(QString type);

  void setDeleted(bool del) override;
  bool getDeleted() const override;
  GraphItemType getItemType() const override { return GraphItemType::kNode; }

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
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
  QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                      const QVariant &value) override;

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
