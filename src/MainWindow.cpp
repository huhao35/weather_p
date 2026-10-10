#include "MainWindow.hpp"
#include "TempModel.hpp"
#include "entity.hpp"
#include "ui_MainWindow.h"
#include <bits/chrono.h>
#include <chrono>
#include <cstdint>
#include <qabstractitemview.h>
#include <qaction.h>
#include <qstackedwidget.h>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  model = std::make_unique<TempModel>(this);
  interface = std::make_unique<DataSource::HttpDataSource>();
  ui->setupUi(this);
  setUpSignal();
   ui->stackedWidget->setCurrentIndex(1);
  ui->tableView->setModel(model.get());
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableView->setAlternatingRowColors(true);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
  ui->tableView->verticalHeader()->setVisible(true);
  ui->stackedWidget->setCurrentIndex(0);  
}
MainWindow::~MainWindow() { delete ui; }
void MainWindow::setUpSignal() {
  connect(ui->switchCur, &QAction::triggered, this, &MainWindow::showCurrent);
  connect(ui->switchHist, &QAction::triggered, this, &MainWindow::showHistory);
  connect(ui->stackedWidget, &QStackedWidget::currentChanged, this,
          [&](int index) {
            if (index == 0) {
              tryUpdateHistory();
            } else if(index == 1){
              tryUpdateCurrent();
            }
          });  
}
void MainWindow::showHistory() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::showCurrent(){ui->stackedWidget->setCurrentIndex(1);}
void MainWindow::tryUpdateCurrent() {
  interface->get_data_current(
      [this](std::optional<TempHumidity> t, std::string s) {
          
    });  
}
void MainWindow::tryUpdateHistory() {
    auto date = std::chrono::floor<std::chrono::days>( std::chrono::system_clock::now()) - std::chrono::days{1};  
    uint64_t time_stamp = std::chrono::duration_cast<std::chrono::seconds>(date.time_since_epoch()).count();
    interface->get_data_date(
        time_stamp,
        [this](std::optional<std::vector<TempHumidity>> t, std::string error) {
          if (t.has_value()) {
            auto r = t.value();
            int size = r.size();
            model->setData(std::move(r));
            QMessageBox::information(this, "Information", QString::fromStdString(std::format("Updated! {} rows affected",size)));            
          } else {
            QMessageBox::warning(
                this, "Warning",
                QString::fromStdString("Update Error: " + error));
          }
        });    
}
