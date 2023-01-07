/**
 * @file iosummary.cpp
 * @brief 
 * @author opluss (opluss@qq.com)
 * 
 * @copyright Copyright (c) 2023  opluss
 * 
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-07 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/ui/dialog/iosummary/iosummary.h"

#include <QPair>
#include <QPushButton>

#include "gui/ui/dialog/helper.h"
#include "gui/ui/dialog/iosummary/ui_iosummary.h"

IOSummary::IOSummary(const QString& label, QString* name, QString* type,
                     QList<int64_t>* dim, QWidget* parent)
    : QDialog(parent), ui(new Ui::IOSummary), name(name), type(type), dim(dim) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->label->adjustSize();
  ui->name_edit->setText(*name);
  ui->type_edit->setText(*type);
  ui->dim_edit->setText(dim2str(*dim));
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &IOSummary::buttonAcceptedSlot);
  connect(ui->dim_edit, &QLineEdit::textChanged, this,
          &IOSummary::dimCheckSlot);
  dimCheckSlot();
}

IOSummary::~IOSummary() { delete ui; }

void IOSummary::dimCheckSlot() {
  auto ret = str2dim(ui->dim_edit->text());
  auto b = ui->buttonBox->button(QDialogButtonBox::Ok);
  if (b != nullptr) {
    b->setEnabled(ret.first);
  }
}

void IOSummary::buttonAcceptedSlot() {
  *name = ui->name_edit->text();
  *type = ui->type_edit->text();
  auto ret = str2dim(ui->dim_edit->text());
  *dim = ret.second;
}
