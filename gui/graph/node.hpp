/**
 * @file node.hpp
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

#ifndef GUI_GRAPH_NODE_HPP_
#define GUI_GRAPH_NODE_HPP_

#include <QGraphicsItem>

#include "gui/config/ui.h"

namespace gui {
namespace graph {

class Node : public QGraphicsItem {
 public:
  struct NodeAttr_t {
    QString key;
    QString value;
    QRectF rect_key;
    QRectF rect_val;
  };

 public:
  explicit Node(config::Ui &cfg);

  void init(const QString &title, const QList<QString> &attr_keys,
            const QList<QString> &attr_vals);

 public:
  QRectF boundingRect() const override;

 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = 0) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

 private:
  config::Ui &mCfg;

  QString mTitle = "";
  QList<NodeAttr_t> mAttrs;

  QRectF mAllRect;
  QRectF mTitleRect;
  QRectF mFirstAttrRect;
  bool mHovered = false;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_NODE_HPP_
