/**
 * @file edge.hpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-28 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_GRAPH_EDGE_H_
#define GUI_GRAPH_EDGE_H_

#include <QGraphicsItem>

#include "gui/graph/context.h"
#include "gui/graph/item.h"
#include "utils/simonnx/context.h"

namespace gui {
namespace graph {

using TensorHandle = utils::simonnx::TensorHandle;

class Edge : public GraphItemBase {
 public:
  explicit Edge(Context& cfg);

  void bind(TensorHandle handle);
  void updateEdge(const QList<QPointF>& src, const QList<QPointF>& dst);
  QString getName() const;
  QList<int64_t> getDim() const;
  QString getDataType() const;

  void setDeleted(bool del) override;
  bool getDeleted() const override;
  GraphItemType getItemType() const override { return GraphItemType::kEdge; }

 protected:
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = 0) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

 private:
  Context& ctx_;
  TensorHandle handle_;

  QList<QPointF> src_;
  QList<QPointF> dst_;

  bool mHovered = false;

  QPainterPath mPath;
  QRectF mNameRect;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_EDGE_H_
