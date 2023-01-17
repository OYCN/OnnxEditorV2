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
#include <functional>

namespace gui {

class MainWindow;

class Actions : public QObject {
  Q_OBJECT

 public:
  explicit Actions(MainWindow* parent);

 public:
  void updateStatus();

 private:
  QMenu* addMenu(const QString& label, QMenu* menu = nullptr);
  QAction* addAction(QMenu* menu, const QString& label,
                     std::function<void()> f);

 private slots:
  void act_file_open_callback();
  void act_file_save_a_callback(QString fileName = "");

  void act_node_summary_dialog_exec_callback();
  void act_io_summary_dialog_exec_callback();
  void act_random_graph_callback();

 private:
  MainWindow* parent_;

  QAction* act_dump2gml_;
  QAction* act_dump2svg_;
  QAction* act_dump2dot_;
};

}  // namespace gui

#endif  // GUI_ACTIONS_H_
