/**
 * @file ogdf_proxy.cpp
 * @brief 
 * @author opluss (opluss@qq.com)
 * 
 * @copyright Copyright (c) 2022  opluss
 * 
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-28 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */
#include <gtest/gtest.h>

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

using namespace utils::algorithm::external::ogdf;

TEST(ogdf_proxy, to_gml) {
  auto g = genRandomGraph(10, 10);
  toGml(g.get(), "out.gml");
}

TEST(ogdf_proxy, to_svg) {
  auto g = genRandomGraph(10, 10);
  toSvg(g.get(), "out.svg", true);
}
