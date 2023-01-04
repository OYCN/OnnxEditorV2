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
#include "utils/simonnx/onnx_pass.h"

using SimOnnxCtx = utils::simonnx::SimOnnxCtx;

namespace gui {

Actions::Actions(MainWindow* parent) : parent_(parent), QObject(parent) {
  auto menu_file = addMenu("File");
  auto menu_debug = addMenu("Debug");
  auto menu_debug_ui = addMenu("ui", menu_debug);
  auto menu_debug_graph = addMenu("graph", menu_debug);
  auto menu_debug_graph_dump = addMenu("dump", menu_debug_graph);

  {
    auto act =
        addAction(menu_file, "New File", [&]() { parent_->scene_->clear(); });
    act->setStatusTip("Create a new onnx file");
    act->setShortcut(QKeySequence("Ctrl+n"));
  }
  {
    auto act =
        addAction(menu_file, "Open File", [&]() { act_file_open_callback(); });
    act->setStatusTip("Open an exist onnx file");
    act->setShortcut(QKeySequence("Ctrl+o"));
  }
  {
    auto act =
        addAction(menu_file, "Save as", [&]() { act_file_save_a_callback(); });
    act->setStatusTip("Save this onnx file as new file");
    act->setShortcut(QKeySequence("Ctrl+e"));
  }
  {
    auto act =
        addAction(menu_file, "Close", [&]() { parent_->scene_->clear(); });
    act->setStatusTip("Close this file");
    act->setShortcut(QKeySequence("Ctrl+w"));
  }

  addAction(menu_debug_ui, "Txt Set Dialog",
            [&]() { act_txt_set_dialog_exec_callback(); });
  addAction(menu_debug_ui, "Txt List Set Dialog",
            [&]() { act_txt_list_set_dialog_exec_callback(); });
  addAction(menu_debug_ui, "Node Summary Dialog",
            [&]() { act_node_summary_dialog_exec_callback(); });

  addAction(menu_debug_graph, "Random Graph",
            [&]() { act_random_graph_callback(); });
  addAction(menu_debug_graph, "pass delete", [&]() {
    if (!utils::simonnx::OnnxPass::remove_deleted_obj(
            SimOnnxCtx::getSimOnnxCtx())) {
      QMessageBox::critical(parent_, tr("Pass Error"), tr("pass delete error"),
                            QMessageBox::Ok);
    }
  });
  addAction(menu_debug_graph, "pass topo sort", [&]() {
    if (!utils::simonnx::OnnxPass::node_topo_sort(
            SimOnnxCtx::getSimOnnxCtx())) {
      QMessageBox::critical(parent_, tr("Pass Error"), tr("pass delete error"),
                            QMessageBox::Ok);
    }
  });
  addAction(menu_debug_graph, "reload graph",
            [&]() { parent_->scene_->loadGraph(SimOnnxCtx::getSimOnnxCtx()); });
  addAction(menu_debug_graph, "layout", [&]() { parent_->scene_->layout(); });
  act_dump2gml_ = addAction(menu_debug_graph_dump, "dump to gml",
                            [&]() { parent_->scene_->dump("debug.gml"); });
  act_dump2svg_ = addAction(menu_debug_graph_dump, "dump to svg",
                            [&]() { parent_->scene_->dump("debug.svg"); });
  act_dump2dot_ = addAction(menu_debug_graph_dump, "dump to dot",
                            [&]() { parent_->scene_->dump("debug.dot"); });
}

void Actions::updateStatus() {
  auto type_str = parent_->view_->getCtx().display.view.layout.toLower();
  auto allowed = [&](QVector<QString> list) {
    for (const auto& v : list) {
      if (v == type_str) {
        return true;
      }
    }
    return false;
  };
  act_dump2gml_->setEnabled(allowed({"ogdf"}));
  act_dump2svg_->setEnabled(allowed({"ogdf", "graphviz"}));
  act_dump2dot_->setEnabled(allowed({"graphviz"}));
}

QMenu* Actions::addMenu(const QString& label, QMenu* menu) {
  if (menu == nullptr) {
    return parent_->menuBar()->addMenu(label);
  } else {
    return menu->addMenu(label);
  }
}

QAction* Actions::addAction(QMenu* menu, const QString& label,
                            std::function<void()> f) {
  auto act = new QAction(parent_);
  act->setText(label);
  connect(act, &QAction::triggered, this, f);
  menu->addAction(act);
  return act;
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
  auto rect = parent_->scene_->layout();
  rect.adjust(0, -50, 0, 50);
  // qDebug() << rect;
  parent_->view_->setSceneRect(rect);
  parent_->view_->setScale(1.5);
  parent_->view_->centertop();
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
  auto rect = parent_->scene_->layout();
  rect.adjust(0, -50, 0, 50);
  parent_->view_->setSceneRect(rect);
  parent_->view_->setScale(1.5);
  parent_->view_->centertop();
}

}  // namespace gui
