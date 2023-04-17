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

#include <QCoreApplication>

#include "gui/graph/view.h"

namespace gui {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), actions_(this) {
  view_ = new graph::View(this);
  scene_ = view_->getScene();
  this->setCentralWidget(view_);
  InitWindow();

  auto args = QCoreApplication::arguments();
  if (args.size() == 2) {
    if (view_->loadFile(args[1])) {
      setTitleFile(args[1]);
    }
  }
}

MainWindow::~MainWindow() {
  if (view_) {
    delete view_;
  }
}

void MainWindow::InitWindow() {
  this->resize(QSize(800, 600));
  setWindowIcon(QIcon(":/img/appicon.ico"));
  setTitleFile();
}

void MainWindow::setTitleFile(QString path) {
  file_path_ = path;
  if (path.size() != 0) {
    setWindowTitle(QString("OnnxEditor(%1)").arg(path));
  } else {
    setWindowTitle("OnnxEditor");
  }
}

}  // namespace gui
