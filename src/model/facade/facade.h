#ifndef ALGORITHMIC_TRADING_MODEL_FACADE_FACADE_H_
#define ALGORITHMIC_TRADING_MODEL_FACADE_FACADE_H_

#include <memory>

#include "../cubic_spline/cubic_spline.h"
#include "../helpers/file_handler.h"
#include "../polynomial_approximation/polynomial_approximation.h"

namespace s21 {

class Facade {
 public:
  Facade() = default;
  bool LoadDataFromFile(const std::string& file);
  [[nodiscard]] const QVector<QCPGraphData>& GetNativeData() const;

  [[nodiscard]] QVector<QCPGraphData> GetApproximationData(
      int polynomial_degree, int number_of_points, int predict_days) const;
  [[nodiscard]] QVector<QCPGraphData> GetInterpolationData(
      int number_of_points) const;

  double GetInterpolationValue(const QDateTime& date_time);
  double GetApproximationValue(const QDateTime& date_time,
                               int polynomial_degree);

  void RefreshCubicSpline();
  void RefreshPolynomialApproximation();

 private:
  QVector<QCPGraphData> native_data_;
  std::shared_ptr<CubicSpline> cubic_spline_;
  std::shared_ptr<PolynomialApproximation> polynomial_approximation_;
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_MODEL_FACADE_FACADE_H_
