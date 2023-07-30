#include "facade.h"

namespace s21 {

bool Facade::LoadDataFromFile(const std::string& file) {
  FileHandler file_reader;
  bool success = file_reader.Parsing(file);
  QVector<QCPGraphData> result = file_reader.GetData();
  if (success) native_data_ = result;
  return success;
}

const QVector<QCPGraphData>& Facade::GetNativeData() const {
  return native_data_;
}

QVector<QCPGraphData> Facade::GetApproximationData(int polynomial_degree,
                                                   int number_of_points,
                                                   int predict_days) const {
  return polynomial_approximation_->GetPredictedData(
      polynomial_degree, number_of_points, predict_days);
}

QVector<QCPGraphData> Facade::GetInterpolationData(int number_of_points) const {
  return cubic_spline_->GetPredictedData(number_of_points);
}

void Facade::RefreshCubicSpline() {
  cubic_spline_ = std::make_shared<CubicSpline>(native_data_);
}

void Facade::RefreshPolynomialApproximation() {
  polynomial_approximation_ =
      std::make_shared<PolynomialApproximation>(native_data_);
}

double Facade::GetInterpolationValue(const QDateTime& date_time) {
  return cubic_spline_->GetPredictedValue(date_time);
}

double Facade::GetApproximationValue(const QDateTime& date_time,
                                     int polynomial_degree) {
  return polynomial_approximation_->GetPredictedValue(date_time,
                                                      polynomial_degree);
}

}  // namespace s21