#include "gui/ui/dialog/txtlistsetdialog/txtlistsetdialog.h"

#include "gui/ui/dialog/txtlistsetdialog/ui_txtlistsetdialog.h"

TxtListSetDialog::TxtListSetDialog(const QString& label, QList<QString>& list,
                                   QWidget* parent)
    : QDialog(parent), ui(new Ui::TxtListSetDialog), list(list) {
  ui->setupUi(this);
  ui->label->setText(label);
  for (int i = 0; i < list.size(); i++) {
    QListWidgetItem* item = new QListWidgetItem(list[i]);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(item);
  }
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &TxtListSetDialog::buttonAcceptedSlot);
}

TxtListSetDialog::~TxtListSetDialog() { delete ui; }

void TxtListSetDialog::buttonAcceptedSlot() {
  list.clear();
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);
    list.append(item->text());
  }
}
