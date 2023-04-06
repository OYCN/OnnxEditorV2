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
                     QList<int64_t>* dim, QWidget* parent,
                     std::function<QList<int64_t>(QString)> name2dim_callback,
                     std::function<QString(QString)> name2type_callback)
    : QDialog(parent),
      ui(new Ui::IOSummary),
      name(name),
      type(type),
      dim(dim),
      name2dim_callback(name2dim_callback),
      name2type_callback(name2type_callback) {
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
  connect(ui->name_edit, &QLineEdit::textChanged, this,
          &IOSummary::name2DimSlot);
  connect(ui->name_edit, &QLineEdit::textChanged, this,
          &IOSummary::name2TypeSlot);
  connect(ui->dim_edit, &QLineEdit::textChanged, this,
          &IOSummary::dimCheckSlot);
  dimCheckSlot();
}

IOSummary::~IOSummary() { delete ui; }

void IOSummary::name2DimSlot() {
  if (name2dim_callback == nullptr) {
    return;
  }
  auto n = ui->name_edit->text();
  auto dim = name2dim_callback(n);
  if (dim.size() != 0) {
    ui->dim_edit->setText(dim2str(dim));
    LOG(INFO) << "set dim to " << ui->dim_edit->text().toStdString();
    return;
  }
  LOG(INFO) << "dim is empty, skip: " << n.toStdString();
}
void IOSummary::name2TypeSlot() {
  if (name2type_callback == nullptr) {
    return;
  }
  auto n = ui->name_edit->text();
  auto type = name2type_callback(n);
  if (type.size() != 0) {
    auto idx = ui->type_edit->findText(type);
    if (idx == -1) {
      LOG(ERROR) << "unknow type: " << type.toStdString();
      return;
    }
    ui->type_edit->setCurrentIndex(idx);
    LOG(INFO) << "set type to " << ui->type_edit->currentText().toStdString();
    return;
  }
  LOG(INFO) << "type is empty, skip: " << n.toStdString();
}
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
