/**
 * @file item.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-27 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_GRAPH_ITEM_H_
#define GUI_GRAPH_ITEM_H_

#include <QGraphicsItem>

namespace gui {
namespace graph {

enum GraphItemType {
  kNode = 0,
  kEdge = 1
};

class GraphItemBase : public QGraphicsItem {
 public:
  virtual void setDeleted(bool del) = 0;
  virtual bool getDeleted() const = 0;
  virtual GraphItemType getItemType() const = 0;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_ITEM_H_
