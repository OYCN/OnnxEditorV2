#ifndef TXTLISTSETDIALOG_H
#define TXTLISTSETDIALOG_H

#include <QDialog>

namespace Ui {
class TxtListSetDialog;
}

class TxtListSetDialog : public QDialog {
  Q_OBJECT

 public:
  explicit TxtListSetDialog(const QString& label, QList<QString>& list,
                            QWidget* parent = nullptr);
  ~TxtListSetDialog();

 public slots:
  void buttonAcceptedSlot();

 private:
  Ui::TxtListSetDialog* ui;
  QList<QString>& list;
};

#endif  // TXTLISTSETDIALOG_H
