/**
 * @file layout.cpp
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

#include "utils/algorithm/layout.h"

#include <glog/logging.h>

#include <magic_enum.hpp>

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

namespace utils {
namespace algorithm {
namespace layout {

GraphNode2LayoutDescTmp Layout::layout(const GraphNode2NodeDesc* g,
                                       LayoutAlgorithm_t t) {
  CHECK_NOTNULL(g);
  switch (t) {
    case LayoutAlgorithm_t::kOGDF:
      return layout_ogdf(g);
    default:
      LOG(FATAL) << "Unsupport layout algorithm: " << magic_enum::enum_name(t);
  }
}

GraphNode2LayoutDescTmp Layout::layout_ogdf(const GraphNode2NodeDesc* g) {
  CHECK_NOTNULL(g);
  return external::ogdf::layout(g);
}

}  // namespace layout
}  // namespace algorithm
}  // namespace utils
