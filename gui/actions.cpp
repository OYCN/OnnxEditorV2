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

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

#include "gui/graph/view.h"
#include "gui/mainwindow.h"
#include "gui/ui/dialog/nodesummary/nodesummary.h"
#include "gui/ui/dialog/txtlistsetdialog/txtlistsetdialog.h"
#include "gui/ui/dialog/txtsetdialog/txtsetdialog.h"
#include "utils/simonnx/context.h"

using SimOnnxCtx = utils::simonnx::SimOnnxCtx;

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

  menu_debug_ = parent->menuBar()->addMenu("Debug");

  act_display_txt_set_dialog_ = new QAction(parent);
  act_display_txt_set_dialog_->setText("Txt Set Dialog");
  connect(act_display_txt_set_dialog_, &QAction::triggered, this,
          &Actions::act_txt_set_dialog_exec_callback);
  act_display_txt_list_set_dialog_ = new QAction(parent);
  act_display_txt_list_set_dialog_->setText("Txt List Set Dialog");
  connect(act_display_txt_list_set_dialog_, &QAction::triggered, this,
          &Actions::act_txt_list_set_dialog_exec_callback);
  act_display_node_summary_dialog_ = new QAction(parent);
  act_display_node_summary_dialog_->setText("Node Summary Dialog");
  connect(act_display_node_summary_dialog_, &QAction::triggered, this,
          &Actions::act_node_summary_dialog_exec_callback);
  menu_debug_ui_ = menu_debug_->addMenu("ui");
  menu_debug_ui_->addAction(act_display_txt_set_dialog_);
  menu_debug_ui_->addAction(act_display_txt_list_set_dialog_);
  menu_debug_ui_->addAction(act_display_node_summary_dialog_);

  act_random_graph_ = new QAction(parent);
  act_random_graph_->setText("Random Graph");
  connect(act_random_graph_, &QAction::triggered, this,
          &Actions::act_random_graph_callback);
  menu_debug_graph_ = menu_debug_->addMenu("graph");
  menu_debug_graph_->addAction(act_random_graph_);
}

void Actions::act_file_new_callback() {
  LOG(INFO) << "act_file_new_callback";
  parent_->scene_->clear();
}

void Actions::act_file_open_callback() {
  LOG(INFO) << "act_file_open_callback";
  QString fileName = QFileDialog::getOpenFileName(parent_, "open onnx file",
                                                  "/", tr("*.onnx"));
  if (fileName.isEmpty()) {
    return;
  }
  // onnx2graph will reset ctx, if not switch ctx obj, we will clear scene
  // before process it
  parent_->scene_->clear();
  SimOnnxCtx::getSimOnnxCtx()->setErrorFn([&](std::string msg) {
    QMessageBox::critical(parent_, tr("Open Error"), tr(msg.c_str()),
                          QMessageBox::Ok);
  });
  bool ret = SimOnnxCtx::getSimOnnxCtx()->openOnnx(fileName.toStdString());
  SimOnnxCtx::getSimOnnxCtx()->resetErrorFn();
  if (!ret) {
    return;
  }
  parent_->scene_->loadGraph(SimOnnxCtx::getSimOnnxCtx());
  parent_->scene_->layout();
  parent_->view_->expand(5);
  parent_->view_->center();
}

void Actions::act_file_save_a_callback() {
  LOG(INFO) << "act_file_save_a_callback";
  QString fileName = QFileDialog::getSaveFileName(parent_, "save onnx file",
                                                  "/", tr("*.onnx"));
  if (fileName.isEmpty()) {
    return;
  }
  SimOnnxCtx::getSimOnnxCtx()->setErrorFn([&](std::string msg) {
    QMessageBox::critical(parent_, tr("Save Error"), tr(msg.c_str()),
                          QMessageBox::Ok);
  });
  bool ret =
      SimOnnxCtx::getSimOnnxCtx()->saveOnnx(fileName.toStdString(), true);
  SimOnnxCtx::getSimOnnxCtx()->resetErrorFn();
  if (!ret) {
    return;
  }
}

void Actions::act_file_close_callback() {
  LOG(INFO) << "act_file_close_callback";
  parent_->scene_->clear();
}

void Actions::act_txt_set_dialog_exec_callback() {
  QString out = "input_data";
  TxtSetDialog d("debug label:", out, parent_);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    LOG(INFO) << "Accepted: " << out.toStdString();
  } else {
    LOG(INFO) << "Rejected";
  }
}

void Actions::act_txt_list_set_dialog_exec_callback() {
  QList<QString> out = {"in0", "in1", "in2"};
  TxtListSetDialog d("debug label:", out, parent_);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    LOG(INFO) << "Accepted:";
    for (const auto& v : out) {
      LOG(INFO) << v.toStdString();
    }
  } else {
    LOG(INFO) << "Rejected";
  }
}

void Actions::act_node_summary_dialog_exec_callback() {
  QString name = "node name";
  QString op_type = "node op type";
  QList<QString> ins = {"in0", "in1", "in2"};
  QList<QString> outs = {"out0", "out1"};
  NodeSummary d(name, op_type, ins, outs, parent_);
  auto ret = d.exec();
  if (ret == QDialog::Accepted) {
    LOG(INFO) << "Accepted:";
    LOG(INFO) << name.toStdString();
    LOG(INFO) << op_type.toStdString();
    for (const auto& v : ins) {
      LOG(INFO) << v.toStdString();
    }
    for (const auto& v : outs) {
      LOG(INFO) << v.toStdString();
    }
  } else {
    LOG(INFO) << "Rejected";
  }
}

void Actions::act_random_graph_callback() {
  LOG(INFO) << "act_random_graph_callback";
  parent_->scene_->clear();
  SimOnnxCtx::getSimOnnxCtx()->genRandomOnnx(50);
  parent_->scene_->loadGraph(SimOnnxCtx::getSimOnnxCtx());
  parent_->scene_->layout();
  parent_->view_->expand(5);
  parent_->view_->center();
}

}  // namespace gui
