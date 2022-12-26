#include "nodesummary.h"

#include "ui_nodesummary.h"

NodeSummary::NodeSummary(QString& name, QString& op_type,
                         QList<QString>& inputs, QList<QString>& outputs,
                         QWidget* parent)
    : QDialog(parent),
      ui(new Ui::NodeSummary),
      name(name),
      op_type(op_type),
      inputs(inputs),
      outputs(outputs) {
  ui->setupUi(this);
  ui->name_edit->setText(name);
  ui->op_type_edit->setText(op_type);
  for (int i = 0; i < inputs.size(); i++) {
    addItem(ui->inputs_list_widget, inputs[i]);
  }
  for (int i = 0; i < outputs.size(); i++) {
    addItem(ui->outputs_list_widget, outputs[i]);
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

void NodeSummary::buttonAcceptedSlot() {
  name = ui->name_edit->text();
  op_type = ui->op_type_edit->text();
  inputs.clear();
  for (int i = 0; i < ui->inputs_list_widget->count(); i++) {
    auto item = ui->inputs_list_widget->item(i);
    inputs.append(item->text());
  }
  outputs.clear();
  for (int i = 0; i < ui->outputs_list_widget->count(); i++) {
    auto item = ui->outputs_list_widget->item(i);
    outputs.append(item->text());
  }
}
