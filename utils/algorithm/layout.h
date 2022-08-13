/**
 * @file layout.h
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

#ifndef UTILS_ALGORITHM_LAYOUT_H_
#define UTILS_ALGORITHM_LAYOUT_H_

#include <vector>

#include "utils/algorithm/graph_desc.h"

namespace utils {
namespace algorithm {
namespace layout {

using GraphNode2NodeDesc = desc::GraphNode2NodeDesc;
using GraphNode2LayoutDescTmp = desc::GraphNode2LayoutDescTmp;

enum class LayoutAlgorithm_t { kOGDF };

class Layout {
 public:
  static GraphNode2LayoutDescTmp layout(const GraphNode2NodeDesc* g,
                                        LayoutAlgorithm_t t);

 private:
  static GraphNode2LayoutDescTmp layout_ogdf(const GraphNode2NodeDesc* g);
};

}  // namespace layout
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_LAYOUT_H_
