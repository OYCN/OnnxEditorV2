/**
 * @file txtsetdialog.h
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

#ifndef GUI_UI_DIALOG_TXTSETDIALOG_TXTSETDIALOG_H_
#define GUI_UI_DIALOG_TXTSETDIALOG_TXTSETDIALOG_H_

#include <QDialog>

namespace Ui {
class TxtSetDialog;
}

class TxtSetDialog : public QDialog {
  Q_OBJECT

 public:
  explicit TxtSetDialog(const QString& label, QString* input,
                        QWidget* parent = nullptr);
  explicit TxtSetDialog(const QString& label, QList<int64_t>* dim,
                        QWidget* parent = nullptr);
  ~TxtSetDialog();

 public slots:
  void dimCheckSlot();
  void buttonAcceptedSlot();

 private:
  Ui::TxtSetDialog* ui;
  QString* input;
  QList<int64_t>* dim;
};

#endif  // GUI_UI_DIALOG_TXTSETDIALOG_TXTSETDIALOG_H_
