#include <QApplication>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::view w = s21::view(new s21::Controller(new s21::Facade()));
  w.show();
  return QApplication::exec();
}
