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
