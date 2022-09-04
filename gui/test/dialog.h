/**
 * @file dialog.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-09-04 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_TEST_DIALOG_H_
#define GUI_TEST_DIALOG_H_

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace gui {
namespace test {

class Dialog : public QDialog {
  Q_OBJECT

 public:
  explicit Dialog(QWidget* parent, QString res);

 private:
  QLabel* mainPic;
  QPushButton* closeBtn;
};

}  // namespace test
}  // namespace gui

#endif  // GUI_TEST_DIALOG_H_
