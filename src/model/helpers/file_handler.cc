//
// Created by Chastity Eryn on 7/25/23.
//

#include "file_handler.h"

namespace s21 {

bool FileHandler::Parsing(const string& path) {
  data_.clear();
  ifstream in(path);
  if (!in.is_open() || in.eof()) return false;

  string line;
  getline(in, line);

  while (getline(in, line)) {
    if (line.empty()) continue;
    if (error_) return false;
    pair<QDateTime, double> date_and_price = GetDateAndPrice(line);
    QCPGraphData graph_data = QCPGraphData(
        static_cast<double>(date_and_price.first.toSecsSinceEpoch()),
        date_and_price.second);
    data_.push_back(graph_data);
  }

  return true;
}

pair<QDateTime, double> FileHandler::GetDateAndPrice(const string& line) {
  pair<QDateTime, double> result{};
  regex regex("[\\-,]");
  vector<string> out(sregex_token_iterator(line.begin(), line.end(), regex, -1),
                     sregex_token_iterator());
  if (out.size() != static_cast<size_t>(4)) error_ = true;

  if (!error_) {
    try {
      int year = std::stoi(out[0]);
      int month = std::stoi(out[1]);
      int day = std::stoi(out[2]);
      double price = std::stod(out[3]);
      result.first = QDateTime(QDate(year, month, day), QTime());
      result.second = price;
    } catch (std::invalid_argument const& ex) {
      error_ = true;
    } catch (std::out_of_range const& ex) {
      error_ = true;
    }
  }

  return result;
}

QVector<QCPGraphData> FileHandler::GetData() { return data_; }

}  // namespace s21