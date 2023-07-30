#ifndef ALGORITHMIC_TRADING_MODEL_HELPERS_MATRIX_H_
#define ALGORITHMIC_TRADING_MODEL_HELPERS_MATRIX_H_

#include <cmath>
#include <cstdarg>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace s21 {
template <class T>
class Matrix {
 public:
  [[nodiscard]] int GetRows() const;
  [[nodiscard]] int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);
  void ResizeMatrix(int rows, int cols);
  bool GetError() { return error_; };
  bool IsEqualMatrix(const Matrix &other);
  void MulMatrix(const Matrix<T> &other);
  void SwapRows(int number_row_1, int number_row_2);

  Matrix() = default;
  Matrix(int rows, int cols);
  Matrix(const Matrix &other);
  [[nodiscard]] Matrix(
      std::initializer_list<T> const &items);  // only for square matrix

  bool operator==(const Matrix &other);
  Matrix &operator=(const Matrix &other);
  T &operator()(int row, int col);
  Matrix<T> operator*(const Matrix<T> &other);

  ~Matrix();

 private:
  int rows_{}, cols_{};
  T **matrix_{};
  bool error_{false};

  void CreateMatrix();
  void DeleteMatrix();
  void CopyMatrix(Matrix const &other);
  bool IsEqualSize(const Matrix &other);
  void InitMatrix(std::initializer_list<T> const &items);
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_MODEL_HELPERS_MATRIX_H_
