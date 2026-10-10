#pragma once
#include "TempModel.hpp"
#include "entity.hpp"
#include <QMainWindow>
#include <qtmetamacros.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  void setUpSignal();
  ~MainWindow() override;
  void showCurrent();
  void showHistory();
  void tryUpdateCurrent();
  void tryUpdateHistory();  
private:
  Ui::MainWindow *ui;
  std::unique_ptr<TempModel> model;  
  std::unique_ptr<DataSource::DataSourceInterface> interface;  
};
