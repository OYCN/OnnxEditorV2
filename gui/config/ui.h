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
    qreal mPadOutsideT = 7;
    qreal mPadOutsideB = 7;
    qreal mPadOutsideL = 3;
    qreal mPadOutsideR = 3;
    qreal mPadTitleT = 1;
    qreal mPadTitleB = 1;
    qreal mPadTitleL = 1;
    qreal mPadTitleR = 1;
    qreal mPadAttrsT = 0.5;
    qreal mPadAttrsB = 0.5;
    qreal mPadAttrsL = 0.5;
    qreal mPadAttrsR = 0.5;
    QString mSpan = ": ";
    QColor mNormalBoundaryColor = {255, 255, 255};
    QColor mSelectedBoundaryColor = {255, 165, 0};
    QColor mGradientColor0 = {128, 128, 128};  // gray
    QColor mGradientColor1 = {80, 80, 80};
    QColor mGradientColor2 = {64, 64, 64};
    QColor mGradientColor3 = {58, 58, 58};
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
