/**
 * @file actions.cpp
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

#include "gui/actions.h"

#include <glog/logging.h>

#include <QDialog>
#include <QFileDialog>
#include <QMenuBar>

#include "gui/graph/view.h"
#include "gui/mainwindow.h"
#include "gui/test/dialog.h"
#include "gui/test/test.h"
#include "utils/simonnx/onnx_meta.h"

namespace gui {

Actions::Actions(MainWindow* parent) : parent_(parent), QObject(parent) {
  act_file_new_ = new QAction(parent);
  act_file_new_->setText("New File");
  act_file_new_->setStatusTip("Create a new onnx file");
  act_file_new_->setShortcut(QKeySequence("Ctrl+n"));
  connect(act_file_new_, &QAction::triggered, this,
          &Actions::act_file_new_callback);

  act_file_open_ = new QAction(parent);
  act_file_open_->setText("Open File");
  act_file_open_->setStatusTip("Open an exist onnx file");
  act_file_open_->setShortcut(QKeySequence("Ctrl+o"));
  connect(act_file_open_, &QAction::triggered, this,
          &Actions::act_file_open_callback);

  act_file_save_as_ = new QAction(parent);
  act_file_save_as_->setText("Save as");
  act_file_save_as_->setStatusTip("Save this onnx file as new file");
  act_file_save_as_->setShortcut(QKeySequence("Ctrl+e"));
  connect(act_file_save_as_, &QAction::triggered, this,
          &Actions::act_file_save_a_callback);

  act_file_close_ = new QAction(parent);
  act_file_close_->setText("Close");
  act_file_close_->setStatusTip("Close this file");
  act_file_close_->setShortcut(QKeySequence("Ctrl+w"));
  connect(act_file_close_, &QAction::triggered, this,
          &Actions::act_file_close_callback);

  menu_file_ = parent->menuBar()->addMenu("File");
  menu_file_->addAction(act_file_new_);
  menu_file_->addAction(act_file_open_);
  menu_file_->addAction(act_file_save_as_);
  menu_file_->addAction(act_file_close_);

  act_show_res_ = new QAction(parent);
  act_show_res_->setText("Show res");
  connect(act_show_res_, &QAction::triggered, this,
          &Actions::act_show_res_callback);
  act_random_graph_ = new QAction(parent);
  act_random_graph_->setText("Random Graph");
  connect(act_random_graph_, &QAction::triggered, this,
          &Actions::act_random_graph_callback);

  menu_debug_ = parent->menuBar()->addMenu("Debug");
  menu_debug_->addAction(act_show_res_);
  menu_debug_->addAction(act_random_graph_);
}

void Actions::act_file_new_callback() {
  LOG(INFO) << "act_file_new_callback";
  QDialog dialog(parent_);
  dialog.setWindowTitle("act_file_new_callback");
  dialog.exec();
}

void Actions::act_file_open_callback() {
  LOG(INFO) << "act_file_open_callback";
  QString fileName = QFileDialog::getOpenFileName(parent_, "open onnx file",
                                                  "/", tr("*.onnx"));
  auto g = utils::simonnx::onnx2graph(fileName.toStdString());
  parent_->scene_->loadGraph(&g);
  parent_->scene_->layout();
  parent_->view_->expand(5);
  parent_->view_->center();
}

void Actions::act_file_save_a_callback() {
  LOG(INFO) << "act_file_save_a_callback";
  QDialog dialog(parent_);
  dialog.setWindowTitle("act_file_save_a_callback");
  dialog.exec();
}

void Actions::act_file_close_callback() {
  LOG(INFO) << "act_file_close_callback";
  QDialog dialog(parent_);
  dialog.setWindowTitle("act_file_close_callback");
  dialog.exec();
}

void Actions::act_show_res_callback() {
  LOG(INFO) << "act_show_res_callback";
  test::Dialog(parent_, "qrc:///BM2.jpg").exec();
  test::Dialog(parent_, "qrc:///res/BM2.jpg").exec();
}

void Actions::act_random_graph_callback() {
  LOG(INFO) << "act_random_graph_callback";
  auto g = test::gen_random_graph(50);
  parent_->scene_->loadGraph(&g);
  parent_->scene_->layout();
  parent_->view_->expand(5);
  parent_->view_->center();
}

}  // namespace gui
