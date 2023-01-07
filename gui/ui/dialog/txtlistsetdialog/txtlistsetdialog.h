/**
 * @file txtlistsetdialog.h
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

#ifndef GUI_UI_DIALOG_TXTLISTSETDIALOG_TXTLISTSETDIALOG_H_
#define GUI_UI_DIALOG_TXTLISTSETDIALOG_TXTLISTSETDIALOG_H_

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class TxtListSetDialog;
}

class TxtListSetDialog : public QDialog {
  Q_OBJECT

 public:
  explicit TxtListSetDialog(const QString& label, QList<QString>* list,
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
  QList<QString>* list;
};

#endif  // GUI_UI_DIALOG_TXTLISTSETDIALOG_TXTLISTSETDIALOG_H_
