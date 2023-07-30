#ifndef VIEW_H
#define VIEW_H

#define AUGMENTATION_FACTOR 4

#include <QMainWindow>
#include <iostream>

#include "controller/controller.h"
#include "view/helper/pages.h"
#include "view/ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class view;
}
QT_END_NAMESPACE

namespace s21 {

class view : public QMainWindow {
  Q_OBJECT

 public:
  explicit view(s21::Controller *controller, QWidget *parent = nullptr);
  ~view() override;

 private slots:
  void ConfirmTimeEntry();
  void ChoseDateFromCalendar(const QDate &date);
  void ChangedTab();
  void LoadDataFromFile();
  void Clear();
  void SetDateTimeRange();
  void GetPrice();
  void PrintGraph();

 private:
  void EnableButtons(bool flag);
  void SetStartSetting();
  void EnableCommonButtons(bool enable);
  void ConnectionButtons();
  void PrintOriginPointsFromFile();
  void SetMinMaxPointsOnGraph(QVector<QCPGraphData> &data);
  void ClearParametersBox(bool success);
  void ReConfigView(bool success);
  void PrintGraphWithLines(const QVector<QCPGraphData> &data,
                           const QString &graph_name);
  QString PrepareAnswer(int points, int type);
  void SetTimeRange();
  void SetFileNameOnInterFace(const QString &fileName);
  void SetTheMaxDegreeOfThePolynomial();
  void SetMaxDate(int type);

  Ui::view *ui_;
  s21::Controller *controller_{};

  bool dataset_loaded = false;
  int graph_counter_ = 0;
  int last_numbers_of_the_days = 0;
};

}  // namespace s21

#endif  // VIEW_H
