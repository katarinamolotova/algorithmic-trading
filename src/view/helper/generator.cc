//
// Created by Rebbecca Gormond on 7/25/23.
//

#include "generator.h"

#include <random>

namespace s21 {

Generator::Generator() = default;

int Generator::GetRandomNumber() {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> dist(static_cast<int>(NUMBER_MIN),
                                          static_cast<int>(NUMBER_MAX));
  return dist(engine);
}
}  // namespace s21