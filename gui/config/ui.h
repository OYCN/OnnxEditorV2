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
  struct View {
    QColor mBackgroundColor = {53, 53, 53};
    QColor mBackgroundFineGridColor = {60, 60, 60};
    QColor mBackgroundCoarseGridColor = {25, 25, 25};
  } view;

  // for node
  struct Node {
    int name_max_width = 150;
    int attr_key_max_width = 80;
    int attr_val_max_width = 70;
    qreal mPadOpTypeT = 1;
    qreal mPadOpTypeB = 1;
    qreal mPadOpTypeL = 5;
    qreal mPadOpTypeR = 5;
    qreal mPadNameT = 1;
    qreal mPadNameB = 1;
    qreal mPadNameL = 5;
    qreal mPadNameR = 5;
    qreal mPadTitleT = 5;
    qreal mPadTitleB = 5;
    qreal mPadAttrsT = 5;
    qreal mPadAttrsB = 5;
    qreal mPadAttrsL = 5;
    qreal mPadAttrsR = 5;
    qreal mPadAttrsGap = 1;
    QColor mNormalBoundaryColor = {255, 255, 255};
    QColor mSelectedBoundaryColor = {255, 165, 0};
    QColor mTitleColor = {80, 80, 80};
    QColor mBgColor = {100, 100, 100};
    qreal mPenWidth = 1;
    qreal mHoveredPenWidth = 1.5;
    QString mFont = "Monospace";
    QColor mFontColor = {255, 255, 255};  // white
  } node;

  // for edge
  struct {
    qreal mEdgeLineWidth = 3.0;
    QColor mEdgeSelectedHallowColor = {255, 165, 0};  // orange
    QColor mEdgeHoveredColor = {224, 255, 255};       // lightcyan
    QColor mEdgeNormalColor = {0, 139, 139};          // darkcyan
    QColor mEdgeSelectedColor = {100, 100, 100};
    qreal mEdgeStartPointRadius = 3;
    QColor mEdgeStartPointColor = {128, 128, 128};  // gray
    qreal mEdgeStopPointRadius = 5;
    QColor mEdgeStopPointColor = {128, 128, 128};  // gray
    QString mEdgeFont = "Monospace";
    QColor mEdgeFontColor = {255, 255, 255};  // white
  } edge;
};

}  // namespace config
}  // namespace gui

#endif  // GUI_CONFIG_UI_H_
