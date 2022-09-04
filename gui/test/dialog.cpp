/**
 * @file dialog.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-09-04 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/test/dialog.h"

#include <glog/logging.h>

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace gui {
namespace test {

Dialog::Dialog(QWidget* parent, QString res)
    : QDialog(parent), mainPic(new QLabel), closeBtn(new QPushButton("Close")) {
  this->setFixedSize(500, 500);
  mainPic->setScaledContents(true);
  LOG(INFO) << "[Dialog] res: " << res.toStdString();
  mainPic->setPixmap(QPixmap(res));

  auto* lay = new QVBoxLayout;
  lay->addWidget(mainPic);

  auto* lay1 = new QHBoxLayout;
  lay1->addStretch(1);
  lay1->addWidget(closeBtn);
  lay->addLayout(lay1);

  this->setLayout(lay);
  connect(closeBtn, &QPushButton::clicked, this,
          [this]() -> void { this->close(); });
}

}  // namespace test
}  // namespace gui
