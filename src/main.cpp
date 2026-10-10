#include <QApplication>
#include "MainWindow.hpp"
int main(int argc, char* argv[]) {
  QApplication e(argc, argv);
  MainWindow mw;
  mw.show();  
  return e.exec();  
}
