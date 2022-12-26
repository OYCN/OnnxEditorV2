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

#include "gui/ui/dialog/txtsetdialog/ui_txtsetdialog.h"

TxtSetDialog::TxtSetDialog(const QString& label, QString& input,
                           QWidget* parent)
    : QDialog(parent), ui(new Ui::TxtSetDialog), input(input) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->lineEdit->setText(input);
  ui->label->adjustSize();
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &TxtSetDialog::buttonAcceptedSlot);
}

TxtSetDialog::~TxtSetDialog() { delete ui; }

void TxtSetDialog::buttonAcceptedSlot() { input = ui->lineEdit->text(); }
