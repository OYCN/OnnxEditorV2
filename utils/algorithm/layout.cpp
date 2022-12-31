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

#include "utils/algorithm/external/graphviz/graphviz_proxy.h"
#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

namespace utils {
namespace algorithm {
namespace layout {

bool endsWith(const std::string& str, const std::string& suffix) {
  if (str.length() < suffix.length()) {
    return false;
  }
  return str.compare(str.length() - suffix.length(), suffix.length(), suffix) ==
         0;
}

GraphNode2LayoutDescTmp Layout::layout(const GraphNode2NodeDesc* g,
                                       LayoutAlgorithm_t t) {
  CHECK_NOTNULL(g);
  switch (t) {
    case LayoutAlgorithm_t::kOGDF:
      return layout_ogdf(g);
    case LayoutAlgorithm_t::kGraphviz:
      return layout_graphviz(g);
    default:
      LOG(FATAL) << "Unsupport layout algorithm: " << magic_enum::enum_name(t);
  }
}

void Layout::dump(const GraphNode2NodeDesc* g, LayoutAlgorithm_t t,
                  std::string out) {
  CHECK_NOTNULL(g);
  if (out.size() == 0) {
    LOG(ERROR) << "out is empty";
    return;
  }
  switch (t) {
    case LayoutAlgorithm_t::kOGDF: {
      if (endsWith(out, ".gml")) {
        external::ogdf::toGml(g, out);
      } else if (endsWith(out, ".svg")) {
        external::ogdf::toSvg(g, out);
      } else {
        LOG(ERROR) << "unsupported suffix in OGDF: " << out;
      }
      break;
    }
    case LayoutAlgorithm_t::kGraphviz: {
      if (endsWith(out, ".dot")) {
        external::graphviz::layout2dot(g, out);
      } else if (endsWith(out, ".svg")) {
        external::graphviz::layout2svg(g, out);
      } else {
        LOG(ERROR) << "unsupported suffix in GraphViz: " << out;
      }
      break;
    }
    default:
      LOG(FATAL) << "Unsupport layout algorithm: " << magic_enum::enum_name(t);
  }
}

GraphNode2LayoutDescTmp Layout::layout_ogdf(const GraphNode2NodeDesc* g) {
  CHECK_NOTNULL(g);
  return external::ogdf::layout(g);
}

GraphNode2LayoutDescTmp Layout::layout_graphviz(const GraphNode2NodeDesc* g) {
  CHECK_NOTNULL(g);
  return external::graphviz::layout(g);
}

}  // namespace layout
}  // namespace algorithm
}  // namespace utils
