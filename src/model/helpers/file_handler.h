//
// Created by Chastity Eryn on 7/25/23.
//

#ifndef ALGORITHMIC_TRADING_FILE_HANDLER_H
#define ALGORITHMIC_TRADING_FILE_HANDLER_H

#include <fstream>
#include <regex>
#include <string>

#include "../../view/qcustomplot.h"

namespace s21 {

using std::ifstream;
using std::pair;
using std::regex;
using std::sregex_token_iterator;
using std::string;
using std::vector;

class FileHandler {
 public:
  FileHandler() = default;
  ~FileHandler() = default;

  bool Parsing(const string& path);
  QVector<QCPGraphData> GetData();

 private:
  QVector<QCPGraphData> data_;
  bool error_ = false;

  pair<QDateTime, double> GetDateAndPrice(const string& line);
};

}  // namespace s21

#endif  // ALGORITHMIC_TRADING_FILE_HANDLER_H
