/**
 * @file ui.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef GUI_CONFIG_UI_H_
#define GUI_CONFIG_UI_H_

#include <QColor>
#include <QString>

namespace gui {
namespace config {

struct Ui {
  // https://www.w3.org/TR/SVG11/types.html#ColorKeywords

  // for view
  QColor mViewBackgroundColor = {53, 53, 53};
  QColor mViewBackgroundFineGridColor = {60, 60, 60};
  QColor mViewBackgroundCoarseGridColor = {25, 25, 25};

  // for node
  qreal mNodePadWL = 7;
  qreal mNodePadWR = 7;
  qreal mNodePadHT = 3;
  qreal mNodePadHB = 3;
  qreal mNodePadNameAttrs = 1;
  qreal mNodePadAttrs = 0.5;
  QColor mNodeNormalBoundarColor = {255, 255, 255};
  QColor mNodeSelectedBoundarColor = {255, 165, 0};
  QColor mNodeGradientColor0 = {128, 128, 128};  // gray
  QColor mNodeGradientColor1 = {80, 80, 80};
  QColor mNodeGradientColor2 = {64, 64, 64};
  QColor mNodeGradientColor3 = {58, 58, 58};
  qreal mNodePenWidth = 1;
  qreal mNodeHoveredPenWidth = 1.5;
  QString mNodeFont = "Monospace";
  QColor mNodeFontColor = {255, 255, 255};  // white

  // for edge
  qreal mEdgeLineWidth = 3.0;
  QColor mEdgeSelectedHaloColor = {255, 165, 0};  // orange
  QColor mEdgeHoveredColor = {224, 255, 255};     // lightcyan
  QColor mEdgeNormalColor = {0, 139, 139};        // darkcyan
  QColor mEdgeSelectedColor = {100, 100, 100};
  qreal mEdgeStartPointRadius = 3;
  QColor mEdgeStartPointColor = {128, 128, 128};  // gray
  qreal mEdgeStopPointRadius = 5;
  QColor mEdgeStopPointColor = {128, 128, 128};  // gray
  QString mEdgeFont = "Monospace";
  QColor mEdgeFontColor = {255, 255, 255};  // white
};

}  // namespace config
}  // namespace gui

#endif  // GUI_CONFIG_UI_H_
