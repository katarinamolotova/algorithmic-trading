//
// Created by anvar on 26.05.23.
//

#include "cubic_spline.h"

namespace s21 {

CubicSpline::CubicSpline(const QVector<QCPGraphData>& source_data)
    : source_data_(source_data) {
  SetSplineCoefficients();
}

void CubicSpline::SetSplineCoefficients() {
  spline_coefficients_ =
      Matrix<long double>(static_cast<int>(source_data_.size() - 1), 4);
  CalculateZerothCoefficients();
  QVector<QCPGraphData> data_delta(source_data_.size() - 1);
  for (int i = 0; i < source_data_.size() - 1; ++i) {
    data_delta[i].key = source_data_[i + 1].key - source_data_[i].key;
    data_delta[i].value = source_data_[i + 1].value - source_data_[i].value;
  }
  CalculateSecondCoefficients(data_delta);
  CalculateThirdCoefficients(data_delta);
  CalculateFirstCoefficients(data_delta);
}

void CubicSpline::CalculateZerothCoefficients() {
  for (int i = 0; i < source_data_.size() - 1; ++i) {
    spline_coefficients_(i, 0) = source_data_[i].value;
  }
}

void CubicSpline::CalculateFirstCoefficients(
    const QVector<QCPGraphData>& data_delta) {
  for (int i = 0; i < spline_coefficients_.GetRows(); ++i) {
    spline_coefficients_(i, 1) =
        data_delta[i].value / data_delta[i].key -
        data_delta[i].key * spline_coefficients_(i, 2) -
        pow(data_delta[i].key, 2) * spline_coefficients_(i, 3);
  }
}

void CubicSpline::CalculateSecondCoefficients(
    const QVector<QCPGraphData>& data_delta) {
  Matrix<long double> tridiagonal_matrix(static_cast<int>(data_delta.size()),
                                         4);
  FillTridiagonalMatrixCoefficients(data_delta, tridiagonal_matrix);
  for (int i = 1; i < tridiagonal_matrix.GetRows(); ++i) {
    long double w = tridiagonal_matrix(i, 0) / tridiagonal_matrix(i - 1, 1);
    tridiagonal_matrix(i, 1) -= w * tridiagonal_matrix(i - 1, 2);
    tridiagonal_matrix(i, 3) -= w * tridiagonal_matrix(i - 1, 3);
  }
  spline_coefficients_(spline_coefficients_.GetRows() - 1, 2) =
      tridiagonal_matrix(tridiagonal_matrix.GetRows() - 1, 3) /
      tridiagonal_matrix(tridiagonal_matrix.GetRows() - 1, 1);
  for (int i = tridiagonal_matrix.GetRows() - 2; i >= 0; --i) {
    spline_coefficients_(i, 2) =
        (tridiagonal_matrix(i, 3) -
         tridiagonal_matrix(i, 2) * spline_coefficients_(i + 1, 2)) /
        tridiagonal_matrix(i, 1);
  }
}

void CubicSpline::CalculateThirdCoefficients(
    const QVector<QCPGraphData>& data_delta) {
  for (int i = 0; i < spline_coefficients_.GetRows() - 1; ++i) {
    spline_coefficients_(i, 3) =
        (spline_coefficients_(i + 1, 2) - spline_coefficients_(i, 2)) /
        (3 * data_delta[i].key);
  }
  spline_coefficients_(spline_coefficients_.GetRows() - 1, 3) =
      -spline_coefficients_(spline_coefficients_.GetRows() - 1, 2) /
      (3 * data_delta[spline_coefficients_.GetRows() - 1].key);
}

void CubicSpline::FillTridiagonalMatrixCoefficients(
    const QVector<QCPGraphData>& data_delta,
    Matrix<long double>& tridiagonal_matrix) {
  tridiagonal_matrix(0, 1) = 1;
  for (int i = 1; i < data_delta.size(); ++i) {
    tridiagonal_matrix(i, 0) = data_delta[i - 1].key;
    tridiagonal_matrix(i, 1) = 2 * (data_delta[i - 1].key + data_delta[i].key);
    tridiagonal_matrix(i, 2) = data_delta[i].key;
    tridiagonal_matrix(i, 3) =
        3 * (data_delta[i].value / data_delta[i].key -
             data_delta[i - 1].value / data_delta[i - 1].key);
  }
  tridiagonal_matrix(tridiagonal_matrix.GetRows() - 1, 2) = 0;
}

QVector<QCPGraphData> CubicSpline::GetPredictedData(int number_of_points) {
  QVector<QCPGraphData> result(number_of_points);
  double x_pred_left = source_data_.first().key;
  double x_pred_right = source_data_.last().key;
  double dx = (x_pred_right - x_pred_left) / (number_of_points - 1);
  int j = 0;
  for (int i = 0; i < number_of_points - 1; ++i) {
    double x_pred = x_pred_left + dx * i;
    while (x_pred > source_data_[j + 1].key) {
      j++;
    }
    result[i].key = x_pred;
    result[i].value = 0;
    for (int degree = 0; degree < 4; ++degree) {
      result[i].value += pow((x_pred - source_data_[j].key), degree) *
                         spline_coefficients_(j, degree);
    }
  }
  result[result.size() - 1].key = x_pred_right;
  result[result.size() - 1].value = source_data_.last().value;
  return result;
}

double CubicSpline::GetPredictedValue(const QDateTime& date_time) {
  auto epoch_time = static_cast<double>(date_time.toSecsSinceEpoch());
  int j = 0;
  while (epoch_time > source_data_[j + 1].key) {
    j++;
  }
  double result = 0;
  for (int degree = 0; degree < 4; ++degree) {
    result += pow((epoch_time - source_data_[j].key), degree) *
              spline_coefficients_(j, degree);
  }
  return result;
}

}  // namespace s21
