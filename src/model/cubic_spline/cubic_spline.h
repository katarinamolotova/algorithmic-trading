//
// Created by anvar on 26.05.23.
//

#ifndef ALGORITHMIC_TRADING_CUBIC_SPLINE_H
#define ALGORITHMIC_TRADING_CUBIC_SPLINE_H

#include <cmath>

#include "../helpers/matrix.h"
#include "view/qcustomplot.h"

namespace s21 {
class CubicSpline {
 public:
  explicit CubicSpline(const QVector<QCPGraphData> &source_data);
  QVector<QCPGraphData> GetPredictedData(int number_of_points);
  double GetPredictedValue(const QDateTime &date_time);

 private:
  QVector<QCPGraphData> source_data_;
  Matrix<long double> spline_coefficients_;

  void SetSplineCoefficients();
  void CalculateZerothCoefficients();
  void CalculateFirstCoefficients(const QVector<QCPGraphData> &data_delta);
  void CalculateSecondCoefficients(const QVector<QCPGraphData> &data_delta);
  void CalculateThirdCoefficients(const QVector<QCPGraphData> &data_delta);
  void FillTridiagonalMatrixCoefficients(
      const QVector<QCPGraphData> &data_delta,
      Matrix<long double> &tridiagonal_matrix);
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_CUBIC_SPLINE_H
