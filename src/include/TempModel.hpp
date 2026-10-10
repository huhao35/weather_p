#pragma once
#include <QAbstractTableModel>
#include <qabstractitemmodel.h>
#include "entity.hpp"
#include <qnamespace.h>
#include <qobject.h>
#include <qvariant.h>
#include <vector>
class TempModel : public QAbstractTableModel {
Q_OBJECT
public:
  TempModel(QObject *parent) : QAbstractTableModel(parent) {}  
  int rowCount(const QModelIndex &parent = QModelIndex())const override;
  int columnCount(const QModelIndex &parent = QModelIndex())const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
    void setData(std::vector<TempHumidity> t);  
private:
  std::vector<TempHumidity> temp_data;
  const char *header[3]{"Temperature", "Humidity", "TimeStamp"};  
};
