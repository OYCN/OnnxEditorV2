/**
 * @file txtsetdialog.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-26 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/ui/dialog/txtsetdialog/txtsetdialog.h"

#include <QPushButton>

#include "gui/ui/dialog/helper.h"
#include "gui/ui/dialog/txtsetdialog/ui_txtsetdialog.h"

TxtSetDialog::TxtSetDialog(const QString& label, QString* input,
                           QWidget* parent)
    : QDialog(parent), ui(new Ui::TxtSetDialog), input(input), dim(nullptr) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->lineEdit->setText(*input);
  ui->label->adjustSize();
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &TxtSetDialog::buttonAcceptedSlot);
}

TxtSetDialog::TxtSetDialog(const QString& label, QList<int64_t>* dim,
                           QWidget* parent)
    : QDialog(parent), ui(new Ui::TxtSetDialog), input(nullptr), dim(dim) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->lineEdit->setText(dim2str(*dim));
  ui->label->adjustSize();
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &TxtSetDialog::buttonAcceptedSlot);
  connect(ui->lineEdit, &QLineEdit::textChanged, this,
          &TxtSetDialog::dimCheckSlot);
  dimCheckSlot();
}

TxtSetDialog::~TxtSetDialog() { delete ui; }

void TxtSetDialog::dimCheckSlot() {
  auto ret = str2dim(ui->lineEdit->text());
  auto b = ui->buttonBox->button(QDialogButtonBox::Ok);
  if (b != nullptr) {
    b->setEnabled(ret.first);
  }
}

void TxtSetDialog::buttonAcceptedSlot() {
  if (input) {
    *input = ui->lineEdit->text();
  }
  if (dim) {
    auto ret = str2dim(ui->lineEdit->text());
    *dim = ret.second;
  }
}
