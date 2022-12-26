/**
 * @file nodesummary.h
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

#ifndef GUI_UI_DIALOG_NODESUMMARY_NODESUMMARY_H_
#define GUI_UI_DIALOG_NODESUMMARY_NODESUMMARY_H_

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class NodeSummary;
}

class NodeSummary : public QDialog {
  Q_OBJECT

 public:
  explicit NodeSummary(QString& name, QString& op_type, QList<QString>& inputs,
                       QList<QString>& outputs, QWidget* parent = nullptr);
  ~NodeSummary();

 public slots:
  void buttonAcceptedSlot();

 private:
  QListWidgetItem* addItem(QListWidget* listWidget, const QString& name);
  void delItem(QListWidget* listWidget, QListWidgetItem* item);

 private:
  Ui::NodeSummary* ui;
  QString& name;
  QString& op_type;
  QList<QString>& inputs;
  QList<QString>& outputs;
};

#endif  // GUI_UI_DIALOG_NODESUMMARY_NODESUMMARY_H_
