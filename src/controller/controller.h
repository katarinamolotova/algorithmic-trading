#ifndef ALGORITHMIC_TRADING_CONTROLLER_CONTROLLER_H_
#define ALGORITHMIC_TRADING_CONTROLLER_CONTROLLER_H_

#include "model/facade/facade.h"
#include "view/helper/pages.h"

namespace s21 {

class Controller {
 public:
  explicit Controller(Facade* model_);
  ~Controller();

  bool LoadDataFromFile(const std::string& file);
  QVector<QCPGraphData> GetData(Pages type, int number_of_points = -1,
                                int polynomial_degree = -1,
                                int predict_days = -1);
  double GetValue(Pages type, const QDateTime& date_time,
                  int polynomial_degree = -1);
  void Refresh(Pages type);

 private:
  Facade* model_;
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_CONTROLLER_CONTROLLER_H_
