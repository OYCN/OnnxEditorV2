/**
 * @file mainwindow.hpp
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

#ifndef GUI_MAINWINDOW_H_
#define GUI_MAINWINDOW_H_

#include <QtWidgets/QMainWindow>

namespace gui {

class Scene;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() {}

 private:
  Scene* mScene = nullptr;
};

}  // namespace gui

#endif  // GUI_MAINWINDOW_H_
