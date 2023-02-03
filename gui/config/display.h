/**
 * @file display.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-09 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_CONFIG_DISPLAY_H_
#define GUI_CONFIG_DISPLAY_H_

#include <QMap>
#include <QSet>
#include <QString>

namespace gui {
namespace config {

struct Display {
  // for view
  struct View {
    // QString layout = "ogdf";
    QString layout = "graphviz";
  } view;

  // for node
  struct Node {
    QSet<QString> hidden_op_type = {"::initializer::", "Constant"};
    QSet<QString> only_display_name = {"::input::", "::output::"};
    QMap<QString, QString> redirect_op_type = {{"::input::", "Input"},
                                               {"::output::", "Output"}};
    bool name = false;
    bool op_type = true;
    bool attr = true;
    bool movable = true;
  } node;

  // for edge
  struct {
    bool name = false;
    bool shape = false;
  } edge;
};

}  // namespace config
}  // namespace gui

#endif  // GUI_CONFIG_DISPLAY_H_
