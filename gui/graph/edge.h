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

#include "gui/config/ui.h"

namespace gui {
namespace graph {

class Node;

class Edge : public QGraphicsItem {
 public:
  explicit Edge(config::Ui& cfg);

  void init(const Node* src, const Node* dst, const QString& label,
            const QPointF& srcp, const QPointF& dstp);
  void updatePoints(const QPointF* srcp = nullptr,
                    const QPointF* dstp = nullptr);
  const std::string getLabel() const { return mLabel.toStdString(); }

 protected:
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = 0) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

 private:
  config::Ui& mCfg;

  bool mHovered = false;

  QString mLabel;

  const Node* mSrc = nullptr;
  const Node* mDst = nullptr;
  QPointF mSrcP;
  QPointF mDstP;

  QPainterPath mPath;
  QRectF mLabelRect;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_EDGE_H_
