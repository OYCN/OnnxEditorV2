#ifndef TXTLISTSETDIALOG_H
#define TXTLISTSETDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

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
  void addButtonClickSlot();
  void delButtonClickSlot();

 private:
  QListWidgetItem* addItem(const QString& name);
  void delItem(QListWidgetItem* item);

 private:
  Ui::TxtListSetDialog* ui;
  QList<QString>& list;
};

#endif  // TXTLISTSETDIALOG_H
