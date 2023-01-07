/**
 * @file helper.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-07 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include <QList>
#include <QPair>
#include <QString>

inline QString dim2str(QList<int64_t> dim) {
  QString dim_str;
  for (size_t i = 0; i < dim.size(); i++) {
    dim_str += QString::number(dim[i]);
    if (i < (dim.size() - 1)) {
      dim_str += ", ";
    }
  }
  return dim_str;
}

inline QPair<bool, QList<int64_t>> str2dim(QString dim_str) {
  dim_str = dim_str.replace(" ", "");
  bool valid = true;
  QList<int64_t> dim;
  if (valid) {
    for (auto d_str : dim_str.split(",")) {
      dim.append(d_str.toLongLong(&valid));
      if (!valid) {
        return {false, dim};
      }
    }
  }
  return {valid, dim};
}