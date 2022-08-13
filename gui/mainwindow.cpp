/**
 * @file mainwindow.cpp
 * @brief
 * @author oPluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  oPluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-04-07 <td>1.0     <td>oPluss     <td>Initial
 * </table>
 */

#include "gui/mainwindow.h"

#include "gui/graph/view.h"

namespace gui {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->resize(QSize(800, 600));
  graph::View *v = new graph::View(this);
  this->setCentralWidget(v);
  v->expand(5);
  v->center();
}

}  // namespace gui
