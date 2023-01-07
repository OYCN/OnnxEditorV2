#ifndef IOSUMMARY_H
#define IOSUMMARY_H

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

#endif  // IOSUMMARY_H
