/**
 * @file view.h
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

#ifndef GUI_GRAPH_VIEW_H_
#define GUI_GRAPH_VIEW_H_

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMenu>

#include "gui/config/ui.h"

namespace gui {
namespace graph {

class Scene;

class View : public QGraphicsView {
  Q_OBJECT

 private:
  enum class MouseStatus { kNone, kPress, kDrag };

 public:
  explicit View(QWidget* parent = nullptr);

 public:
  void expand(qreal f);
  void center();
  void setCfg(config::Ui cfg);
  config::Ui getCfg() { return mCfg; }

 private:
  void updateCfg();

 public Q_SLOTS: // NOLINT
  void scaleUp();
  void scaleDown();

 protected:
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void drawBackground(QPainter* painter, const QRectF& r) override;

 private:
  config::Ui mCfg;

  Scene* mScene = nullptr;
  QPointF click_pos_;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_VIEW_H_
