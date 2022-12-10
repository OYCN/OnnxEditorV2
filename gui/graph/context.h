/**
 * @file context.h
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

#ifndef GUI_GRAPH_CONTEXT_H_
#define GUI_GRAPH_CONTEXT_H_

#include "gui/config/display.h"
#include "gui/config/ui.h"

namespace gui {
namespace graph {

class Context {
 public:
  gui::config::Ui ui;
  gui::config::Display display;
  QWidget* top_widget;
};

}  // namespace graph
}  // namespace gui

#endif  // GUI_GRAPH_CONTEXT_H_
