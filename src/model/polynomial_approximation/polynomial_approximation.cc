//
// Created by Gwynesse Tandy on 5/26/23.
//

#include "polynomial_approximation.h"

namespace s21 {

PolynomialApproximation::PolynomialApproximation(
    const QVector<QCPGraphData>& input_data) {
  input_data_ = input_data;
  scaling_shift_ = input_data_.first().key;
  scaling_factor_ = input_data_.last().key - input_data_.first().key;
  DateShift();
  current_polynomial_degree_ = -1;
}

void PolynomialApproximation::DateShift() {
  for (auto& point : input_data_) {
    point.key -= scaling_shift_;
    point.key *= 1 / scaling_factor_;
  }
}

QVector<QCPGraphData> PolynomialApproximation::GetPredictedData(
    int polynomial_degree, int number_of_points, int predict_days) {
  CheckPolynomialDegree(polynomial_degree);
  double x_pred_left = input_data_.first().key;
  double x_pred_right =
      input_data_.last().key + (predict_days * 24 * 3600 / scaling_factor_);
  return FillPredictedData(number_of_points, x_pred_left, x_pred_right);
}

void PolynomialApproximation::CheckPolynomialDegree(int polynomial_degree) {
  if (current_polynomial_degree_ != polynomial_degree) {
    current_polynomial_degree_ = polynomial_degree;
    polynomial_coefficients_ = GetPolynomialCoefficients(polynomial_degree);
  }
}

std::vector<long double> PolynomialApproximation::GetPolynomialCoefficients(
    int polynomial_degree) {
  Matrix<long double> linear_system_matrix(polynomial_degree + 1,
                                           polynomial_degree + 2);
  FillSystemMatrixCoefficients(polynomial_degree, linear_system_matrix);
  FillConstantTerms(polynomial_degree, linear_system_matrix);
  return SolveSLE(linear_system_matrix);
}

void PolynomialApproximation::FillSystemMatrixCoefficients(
    int polynomial_degree, Matrix<long double>& linear_system_matrix) {
  std::vector<long double> matrix_coefficients(2 * polynomial_degree + 1, 0);
  matrix_coefficients[0] = input_data_.size();
  for (auto point : input_data_) {
    long double temp_x_pow_factor = 1;
    for (int degree = 1; degree <= 2 * polynomial_degree; ++degree) {
      temp_x_pow_factor *= point.key;
      matrix_coefficients[degree] += temp_x_pow_factor;
    }
  }
  std::reverse(matrix_coefficients.begin(), matrix_coefficients.end());
  for (int i = 0; i <= 2 * polynomial_degree; ++i) {
    for (int j = 0; j <= i; ++j) {
      if (i - j <= polynomial_degree && j <= polynomial_degree) {
        linear_system_matrix(i - j, j) =
            matrix_coefficients[2 * polynomial_degree - i];
      }
    }
  }
}

void PolynomialApproximation::FillConstantTerms(
    int polynomial_degree, Matrix<long double>& linear_system_matrix) {
  for (auto point : input_data_) {
    long double temp_y_x_pow_factor = point.value;
    linear_system_matrix(polynomial_degree, polynomial_degree + 1) +=
        temp_y_x_pow_factor;
    for (int degree = 1; degree <= polynomial_degree; ++degree) {
      temp_y_x_pow_factor *= point.key;
      linear_system_matrix(polynomial_degree - degree, polynomial_degree + 1) +=
          temp_y_x_pow_factor;
    }
  }
  for (int i = 0; i < (polynomial_degree + 1) / 2; ++i) {
    std::swap(
        linear_system_matrix(i, polynomial_degree + 1),
        linear_system_matrix(polynomial_degree - i, polynomial_degree + 1));
  }
}

QVector<QCPGraphData> PolynomialApproximation::FillPredictedData(
    int number_of_points, double x_pred_left, double x_pred_right) {
  QVector<QCPGraphData> result(number_of_points);
  for (int i = 0; i < number_of_points; ++i) {
    double x_pred =
        x_pred_left + (x_pred_right - x_pred_left) * i / (number_of_points - 1);
    result[i].key = DateShiftBack(x_pred);
    result[i].value = FillPredictedValue(x_pred);
  }
  return result;
}

double PolynomialApproximation::DateShiftBack(double value) const {
  value *= scaling_factor_;
  value += scaling_shift_;
  return value;
}

double PolynomialApproximation::FillPredictedValue(double x_pred) {
  long double temp_value = polynomial_coefficients_.back();
  long double temp_x_pow_factor = 1;
  for (unsigned int degree = 1; degree < polynomial_coefficients_.size();
       ++degree) {
    temp_x_pow_factor *= x_pred;
    temp_value +=
        temp_x_pow_factor *
        polynomial_coefficients_[polynomial_coefficients_.size() - degree - 1];
  }
  return (double)(temp_value);
}

double PolynomialApproximation::GetPredictedValue(const QDateTime& date_time,
                                                  int polynomial_degree) {
  CheckPolynomialDegree(polynomial_degree);
  auto epoch_time = static_cast<double>(date_time.toSecsSinceEpoch());
  return FillPredictedValue((epoch_time - scaling_shift_) / scaling_factor_);
}

void PolynomialApproximation::MakeForwardElimination(
    Matrix<long double>& matrix) {
  int n = matrix.GetRows();
  for (int k = 0; k < n - 1; ++k) {
    for (int i = k; i < n; ++i) {
      if (matrix(i, k) > matrix(k, k)) {
        matrix.SwapRows(i, k);
      }
    }
    for (int j = k + 1; j < n; ++j) {
      long double m = matrix(j, k) / matrix(k, k);
      for (int i = k; i < n + 1; ++i) {
        matrix(j, i) = matrix(j, i) - m * matrix(k, i);
      }
    }
  }
}

std::vector<long double> PolynomialApproximation::MakeBackSubstitution(
    Matrix<long double>& matrix) {
  int n = matrix.GetRows();
  std::vector<long double> x(n);
  for (int i = n - 1; i >= 0; --i) {
    long double solved_value = 0;
    for (int c = n - 1; c > i; --c) {
      solved_value += matrix(i, c) * x[c];
    }
    x[i] = (matrix(i, n) - solved_value) / matrix(i, i);
  }
  std::reverse(x.begin(), x.end());
  return x;
}

std::vector<long double> PolynomialApproximation::SolveSLE(
    Matrix<long double> matrix) {
  MakeForwardElimination(matrix);
  return MakeBackSubstitution(matrix);
}

}  // namespace s21
