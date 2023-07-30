#include "controller.h"

namespace s21 {

Controller::Controller(Facade* model_) : model_(model_) {}

Controller::~Controller() { delete model_; }

bool Controller::LoadDataFromFile(const std::string& file) {
  return model_->LoadDataFromFile(file);
}

void Controller::Refresh(Pages type) {
  if (type == Pages::kInterpolation)
    model_->RefreshCubicSpline();
  else if (type == Pages::kApproximation)
    model_->RefreshPolynomialApproximation();
}

QVector<QCPGraphData> Controller::GetData(Pages type, int number_of_points,
                                          int polynomial_degree,
                                          int predict_days) {
  QVector<QCPGraphData> result;
  if (type == Pages::kInterpolation)
    result = model_->GetInterpolationData(number_of_points);
  else if (type == Pages::kApproximation)
    result = model_->GetApproximationData(polynomial_degree, number_of_points,
                                          predict_days);
  else if (type == Pages::kAll)
    result = model_->GetNativeData();
  return result;
}

double Controller::GetValue(Pages type, const QDateTime& date_time,
                            int polynomial_degree) {
  double result;
  if (type == Pages::kInterpolation)
    result = model_->GetInterpolationValue(date_time);
  else
    result = model_->GetApproximationValue(date_time, polynomial_degree);
  return result;
}

}  // namespace s21
