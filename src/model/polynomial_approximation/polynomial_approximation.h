//
// Created by Gwynesse Tandy on 5/26/23.
//

#ifndef ALGORITHMIC_TRADING_POLYNOMIAL_APPROXIMATION_H
#define ALGORITHMIC_TRADING_POLYNOMIAL_APPROXIMATION_H

#include <vector>

#include "../helpers/matrix.h"
#include "view/qcustomplot.h"

namespace s21 {
class PolynomialApproximation {
 public:
  explicit PolynomialApproximation(const QVector<QCPGraphData> &input_data);

  QVector<QCPGraphData> GetPredictedData(int polynomial_degree,
                                         int number_of_points,
                                         int predict_days);
  double GetPredictedValue(const QDateTime &date_time, int polynomial_degree);

 private:
  double scaling_shift_;
  double scaling_factor_;
  QVector<QCPGraphData> input_data_;
  int current_polynomial_degree_;
  std::vector<long double> polynomial_coefficients_;

  void CheckPolynomialDegree(int polynomial_degree);

  void DateShift();
  [[nodiscard]] double DateShiftBack(double value) const;
  std::vector<long double> GetPolynomialCoefficients(int polynomial_degree);
  void FillSystemMatrixCoefficients(int polynomial_degree,
                                    Matrix<long double> &linear_system_matrix);
  void FillConstantTerms(int polynomial_degree,
                         Matrix<long double> &linear_system_matrix);
  QVector<QCPGraphData> FillPredictedData(int number_of_points,
                                          double x_pred_left,
                                          double x_pred_right);
  double FillPredictedValue(double x_pred);

  static void MakeForwardElimination(Matrix<long double> &matrix);
  static std::vector<long double> MakeBackSubstitution(
      Matrix<long double> &matrix);
  static std::vector<long double> SolveSLE(Matrix<long double> matrix);
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_POLYNOMIAL_APPROXIMATION_H
