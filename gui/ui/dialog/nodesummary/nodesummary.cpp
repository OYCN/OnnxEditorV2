/**
 * @file nodesummary.cpp
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

#include "gui/ui/dialog/nodesummary/nodesummary.h"

#include <glog/logging.h>

#include <QComboBox>
#include <algorithm>

#include "gui/ui/dialog/nodesummary/ui_nodesummary.h"

NodeSummary::NodeSummary(QString* name, QString* op_type,
                         QList<QString>* inputs, QList<QString>* outputs,
                         QList<QList<QString>>* attrs, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::NodeSummary),
      name(name),
      op_type(op_type),
      inputs(inputs),
      outputs(outputs),
      attrs(attrs) {
  ui->setupUi(this);
  ui->name_edit->setText(*name);
  ui->op_type_edit->setText(*op_type);

  ui->inputs_list_widget->setAlternatingRowColors(true);
  for (int i = 0; i < inputs->size(); i++) {
    addItem(ui->inputs_list_widget, (*inputs)[i]);
  }
  for (int i = 0; i < outputs->size(); i++) {
    addItem(ui->outputs_list_widget, (*outputs)[i]);
  }

  QStringList header;
  header << "name"
         << "type"
         << "value";
  ui->attr_tabel_widget->setColumnCount(header.size());
  ui->attr_tabel_widget->verticalHeader()->hide();
  ui->attr_tabel_widget->verticalHeader()->setDefaultSectionSize(15);
  ui->attr_tabel_widget->setHorizontalHeaderLabels(header);
  // ui->attr_tabel_widget->horizontalHeader()->setStretchLastSection(true);
  ui->attr_tabel_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->attr_tabel_widget->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->attr_tabel_widget->setAlternatingRowColors(true);
  ui->attr_tabel_widget->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Stretch);
  ui->attr_tabel_widget->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::Stretch);
  ui->attr_tabel_widget->horizontalHeader()->setSectionResizeMode(
      2, QHeaderView::Stretch);

  for (int i = 0; i < attrs->size(); i++) {
    addRow(ui->attr_tabel_widget, (*attrs)[i]);
  }

  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &NodeSummary::buttonAcceptedSlot);
  connect(ui->inputs_add, &QPushButton::clicked, this,
          [&]() { addItem(ui->inputs_list_widget, ""); });
  connect(ui->inputs_del, &QPushButton::clicked, this, [&]() {
    auto items = ui->inputs_list_widget->selectedItems();
    for (auto item : items) {
      delItem(ui->inputs_list_widget, item);
    }
  });
  connect(ui->outputs_add, &QPushButton::clicked, this,
          [&]() { addItem(ui->outputs_list_widget, ""); });
  connect(ui->outputs_del, &QPushButton::clicked, this, [&]() {
    auto items = ui->outputs_list_widget->selectedItems();
    for (auto item : items) {
      delItem(ui->outputs_list_widget, item);
    }
  });
  connect(ui->attr_add, &QPushButton::clicked, this, [&]() {
    addRow(ui->attr_tabel_widget, {"", "", ""});
  });
  connect(ui->attr_del, &QPushButton::clicked, this, [&]() {
    auto items = ui->attr_tabel_widget->selectedItems();
    QSet<int> rows_s;
    for (auto item : items) {
      rows_s.insert(item->row());
    }
    delRow(ui->attr_tabel_widget, rows_s);
  });
}

NodeSummary::~NodeSummary() { delete ui; }

QListWidgetItem* NodeSummary::addItem(QListWidget* listWidget,
                                      const QString& name) {
  QListWidgetItem* item = new QListWidgetItem(name);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  listWidget->addItem(item);
  listWidget->setCurrentItem(item);
  return item;
}

void NodeSummary::delItem(QListWidget* listWidget, QListWidgetItem* item) {
  listWidget->removeItemWidget(item);
  delete item;
}

QList<QTableWidgetItem*> NodeSummary::addRow(QTableWidget* listWidget,
                                             const QList<QString>& attr) {
  int row = ui->attr_tabel_widget->rowCount();
  ui->attr_tabel_widget->insertRow(row);
  QList<QTableWidgetItem*> ret;
  for (size_t i = 0; i < attr.size(); i++) {
    if (i == 1) {
      // type
      auto box = new QComboBox();
      QStringList strList;
      strList << "UNDEFINED"
              << "FLOAT"
              << "INT"
              << "FLOATS"
              << "INTS"
              << "STRING";
      box->addItems(strList);
      auto idx = box->findText(attr[i]);
      if (idx == -1) {
        idx = 0;
        if (attr[i].size() != 0) {
          LOG(ERROR) << "unknow type: " << attr[i].toStdString();
        }
      }
      box->setCurrentIndex(idx);
      ui->attr_tabel_widget->setCellWidget(row, i, box);
    } else {
      auto item = new QTableWidgetItem(attr[i]);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
      ret.append(item);
      ui->attr_tabel_widget->setItem(row, i, item);
    }
  }
  ui->attr_tabel_widget->setCurrentCell(row, QItemSelectionModel::Select);
  return ret;
}

void NodeSummary::delRow(QTableWidget* listWidget, QSet<int> rows) {
  auto rows_l = QList<int>(rows.begin(), rows.end());
  std::sort(rows_l.begin(), rows_l.end(), std::greater<int>());
  for (auto row : rows_l) {
    ui->attr_tabel_widget->removeRow(row);
  }
}

void NodeSummary::buttonAcceptedSlot() {
  *name = ui->name_edit->text();
  *op_type = ui->op_type_edit->text();
  inputs->clear();
  for (int i = 0; i < ui->inputs_list_widget->count(); i++) {
    auto item = ui->inputs_list_widget->item(i);
    inputs->append(item->text());
  }
  outputs->clear();
  for (int i = 0; i < ui->outputs_list_widget->count(); i++) {
    auto item = ui->outputs_list_widget->item(i);
    outputs->append(item->text());
  }
  attrs->clear();
  auto row = ui->attr_tabel_widget->rowCount();
  auto col = ui->attr_tabel_widget->columnCount();
  for (int r = 0; r < row; r++) {
    QList<QString> this_r;
    for (int c = 0; c < col; c++) {
      if (c == 1) {
        auto box =
            dynamic_cast<QComboBox*>(ui->attr_tabel_widget->cellWidget(r, c));
        this_r.append(box->currentText());
      } else {
        auto item = ui->attr_tabel_widget->item(r, c);
        this_r.append(item->text());
      }
    }
    attrs->append(this_r);
  }
}
