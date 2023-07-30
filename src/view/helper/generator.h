//
// Created by Rebbecca Gormond on 7/25/23.
//

#ifndef SRC_GENERATOR_H
#define SRC_GENERATOR_H

#define NUMBER_MIN 0
#define NUMBER_MAX 255

namespace s21 {
class Generator {
 public:
  static int GetRandomNumber();

 private:
  Generator();
};
}  // namespace s21

#endif  // SRC_GENERATOR_H
