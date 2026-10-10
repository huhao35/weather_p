#include "TempModel.hpp"
#include <qnamespace.h>
#include <qvariant.h>
#include "utils.hpp"
int TempModel::rowCount(const QModelIndex &parent) const {
    return temp_data.size() + 1;
}
int TempModel::columnCount(const QModelIndex &parent) const { return 3; }
QVariant TempModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};
    if (index.row() < 0 || index.row() >= temp_data.size()) return {};

    const TempHumidity &item = temp_data[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return item.get_temp();
            case 1: return item.get_humidity();
        case 2: return QString::fromStdString(formatTimestamp(item.get_time_stamp()));
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return {};
}
QVariant TempModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (role != Qt::DisplayRole)
    return {};
  if (orientation == Qt::Orientation::Horizontal && section < 3)
    return header[section];
  if (orientation == Qt::Orientation::Horizontal){
    return {};
  }
  return section + 1;  
}
void TempModel::setData(std::vector<TempHumidity> t) {
  beginResetModel();
  temp_data = t;
  endResetModel();  
}

