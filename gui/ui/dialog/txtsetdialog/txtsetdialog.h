#ifndef TXTSETDIALOG_H
#define TXTSETDIALOG_H

#include <QDialog>

namespace Ui {
class TxtSetDialog;
}

class TxtSetDialog : public QDialog {
  Q_OBJECT

 public:
  explicit TxtSetDialog(const QString& label, QString& input,
                        QWidget* parent = nullptr);
  ~TxtSetDialog();

 public slots:
  void buttonAcceptedSlot();

 private:
  Ui::TxtSetDialog* ui;
  QString& input;
};

#endif  // TXTSETDIALOG_H
