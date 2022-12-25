#include "gui/ui/dialog/txtlistsetdialog/txtlistsetdialog.h"

#include "gui/ui/dialog/txtlistsetdialog/ui_txtlistsetdialog.h"

TxtListSetDialog::TxtListSetDialog(const QString& label, QList<QString>& list,
                                   QWidget* parent)
    : QDialog(parent), ui(new Ui::TxtListSetDialog), list(list) {
  ui->setupUi(this);
  ui->label->setText(label);
  ui->label->adjustSize();
  for (int i = 0; i < list.size(); i++) {
    addItem(list[i]);
  }
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &TxtListSetDialog::buttonAcceptedSlot);
  connect(ui->addButton, &QPushButton::clicked, this,
          &TxtListSetDialog::addButtonClickSlot);
  connect(ui->delButton, &QPushButton::clicked, this,
          &TxtListSetDialog::delButtonClickSlot);
}

TxtListSetDialog::~TxtListSetDialog() { delete ui; }

void TxtListSetDialog::buttonAcceptedSlot() {
  list.clear();
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);
    list.append(item->text());
  }
}

void TxtListSetDialog::addButtonClickSlot() { addItem(""); }

void TxtListSetDialog::delButtonClickSlot() {
  auto items = ui->listWidget->selectedItems();
  for (auto item : items) {
    delItem(item);
  }
}

QListWidgetItem* TxtListSetDialog::addItem(const QString& name) {
  QListWidgetItem* item = new QListWidgetItem(name);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  ui->listWidget->addItem(item);
  return item;
}

void TxtListSetDialog::delItem(QListWidgetItem* item) {
  ui->listWidget->removeItemWidget(item);
  delete item;
}
