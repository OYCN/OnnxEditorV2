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

#include <QMainWindow>

#include "gui/actions.h"
#include "gui/ui/sidebar/findsidebar/findsidebar.h"

namespace gui {

namespace graph {
class Scene;
class View;
}  // namespace graph

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() {}

 private:
  void InitWindow();
  void setTitleFile(QString path = "");

 private:
  QWidget* wrapper_widget_ = nullptr;
  FindSidebar* find_siderbar_ = nullptr;
  graph::View* view_ = nullptr;
  graph::Scene* scene_ = nullptr;
  Actions actions_;

  QString file_path_;

  friend class Actions;
};

}  // namespace gui

#endif  // GUI_MAINWINDOW_H_
