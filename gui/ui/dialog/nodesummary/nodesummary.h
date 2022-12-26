#ifndef NODESUMMARY_H
#define NODESUMMARY_H

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

#endif  // NODESUMMARY_H
