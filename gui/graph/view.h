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

#include <QGraphicsView>
#include <QMenu>

#include "gui/graph/context.h"
#include "gui/graph/scene.h"

namespace gui {
namespace graph {

class Scene;

class View : public QGraphicsView {
  Q_OBJECT

 private:
  enum class MouseStatus { kNone, kPress, kDrag };

 public:
  explicit View(QWidget* parent = nullptr);

  bool closeFile();
  bool loadFile(QString path);
  bool saveFile(QString path, bool overwrite = true);

 public:
  void expand(qreal f);
  void expandAbs(qreal w, qreal h);
  void center();
  void centertop();
  const Context& getCtx() const { return ctx_; }
  Scene* getScene();
  void focusOn(GraphItemBase* item, bool clean = true);

 public Q_SLOTS:  // NOLINT
  void scaleUp();
  void scaleDown();
  void scaleExtremeUp();
  void scaleExtremeDown();
  void setScale(qreal scale);

 protected:
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void drawBackground(QPainter* painter, const QRectF& r) override;

 private:
  Context ctx_;

  Scene* scene_ = nullptr;
  QPointF click_pos_;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_VIEW_H_
