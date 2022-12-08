/**
 * @file gui.cpp
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

#include "gui/gui.h"

#include <glog/logging.h>

#include <QApplication>
#include <QFontDatabase>

#include "gui/mainwindow.h"

namespace gui {

int gui_on(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // QString fontPath = ":/fonts/Monospace.ttf";
  QString fontPath = ":/fonts/UbuntuMono-R.ttf";
  int fontId = QFontDatabase::addApplicationFont(fontPath);
  CHECK_NE(fontId, -1);
  QFont font("Monospace");
  app.setFont(font);
  MainWindow w;
  w.show();
  return app.exec();
}

}  // namespace gui
