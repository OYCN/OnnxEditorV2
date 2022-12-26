/**
 * @file actions.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-09-03 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_ACTIONS_H_
#define GUI_ACTIONS_H_

#include <QAction>
#include <QMainWindow>

namespace gui {

class MainWindow;

class Actions : public QObject {
  Q_OBJECT

 public:
  explicit Actions(MainWindow* parent);

 private slots:
  void act_file_new_callback();
  void act_file_open_callback();
  void act_file_save_a_callback();
  void act_file_close_callback();

  void act_txt_list_set_dialog_exec_callback();
  void act_txt_set_dialog_exec_callback();
  void act_node_summary_dialog_exec_callback();
  void act_random_graph_callback();

 private:
  MainWindow* parent_;

  QMenu* menu_file_;
  QAction* act_file_new_;
  QAction* act_file_open_;
  QAction* act_file_save_as_;
  QAction* act_file_close_;

  QMenu* menu_debug_;
  QMenu* menu_debug_ui_;
  QAction* act_display_txt_set_dialog_;
  QAction* act_display_txt_list_set_dialog_;
  QAction* act_display_node_summary_dialog_;
  QMenu* menu_debug_graph_;
  QAction* act_random_graph_;
};

}  // namespace gui

#endif  // GUI_ACTIONS_H_
