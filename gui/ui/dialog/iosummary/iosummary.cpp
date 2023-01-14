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

#include "gui/ui/dialog/iosummary/ui_iosummary.h"
#include "utils/algorithm/parse.h"

using DimStr = utils::algorithm::parse::DimStr;

QString dim2str(QList<int64_t> dim) {
  std::vector<int64_t> d(dim.begin(), dim.end());
  auto str = DimStr::Array2Str(d);
  return QString::fromStdString(str);
}

QList<int64_t> str2dim(QString str) {
  std::string s = str.toStdString();
  auto dim = DimStr::Str2Array(s);
  return {dim.begin(), dim.end()};
}

bool valid(QString str) {
  std::string s = str.toStdString();
  return DimStr::StrValid(s);
}

IOSummary::IOSummary(const QString& label, QString* name, QString* type,
                     QList<int64_t>* dim, QWidget* parent)
    : QDialog(parent), ui(new Ui::IOSummary), name(name), type(type), dim(dim) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->label->adjustSize();
  ui->name_edit->setText(*name);
  ui->dim_edit->setText(dim2str(*dim));
  ui->type_edit->clear();
  QStringList strList;
  strList << "FLOAT"
          << "UINT8"
          << "INT8"
          << "UINT16"
          << "INT16"
          << "UINT32"
          << "INT32"
          << "UINT64"
          << "INT64"
          << "STRING"
          << "BOOL"
          << "FLOAT16"
          << "DOUBLE";
  ui->type_edit->addItems(strList);
  auto idx = ui->type_edit->findText(*type);
  if (idx == -1) {
    idx = 0;
    if (type->size() != 0) {
      LOG(ERROR) << "unknow type: " << type;
    }
  }
  ui->type_edit->setCurrentIndex(idx);
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &IOSummary::buttonAcceptedSlot);
  connect(ui->dim_edit, &QLineEdit::textChanged, this,
          &IOSummary::dimCheckSlot);
  dimCheckSlot();
}

IOSummary::~IOSummary() { delete ui; }

void IOSummary::dimCheckSlot() {
  auto b = ui->buttonBox->button(QDialogButtonBox::Ok);
  if (b != nullptr) {
    b->setEnabled(valid(ui->dim_edit->text()));
  }
}

void IOSummary::buttonAcceptedSlot() {
  *name = ui->name_edit->text();
  *type = ui->type_edit->currentText();
  *dim = str2dim(ui->dim_edit->text());
}
