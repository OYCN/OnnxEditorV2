/**
 * @file iosummary.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-07 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_UI_DIALOG_IOSUMMARY_IOSUMMARY_H_
#define GUI_UI_DIALOG_IOSUMMARY_IOSUMMARY_H_

#include <QDialog>
#include <QList>

namespace Ui {
class IOSummary;
}

class IOSummary : public QDialog {
  Q_OBJECT

 public:
  explicit IOSummary(const QString& label, QString* name, QString* type,
                     QList<int64_t>* dim, QWidget* parent = nullptr);
  ~IOSummary();

 public slots:
  void dimCheckSlot();
  void buttonAcceptedSlot();

 private:
  Ui::IOSummary* ui;

  QString* name;
  QString* type;
  QList<int64_t>* dim;
};

#endif  // GUI_UI_DIALOG_IOSUMMARY_IOSUMMARY_H_
