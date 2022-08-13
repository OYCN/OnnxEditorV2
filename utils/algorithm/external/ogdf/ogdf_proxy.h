/**
 * @file ogdf_proxy.h
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

#ifndef UTILS_ALGORITHM_EXTERNAL_OGDF_OGDF_PROXY_H_
#define UTILS_ALGORITHM_EXTERNAL_OGDF_OGDF_PROXY_H_

#include <string>

#include "utils/algorithm/graph_desc.h"

namespace utils {
namespace algorithm {
namespace external {
namespace ogdf {

using GraphNode2NodeDesc = desc::GraphNode2NodeDesc;
using GraphNode2NodeDescTmp = desc::GraphNode2NodeDescTmp;
using GraphNode2LayoutDescTmp = desc::GraphNode2LayoutDescTmp;

std::unique_ptr<GraphNode2NodeDescTmp> genRandomGraph(size_t n, size_t m);
void toGml(const GraphNode2NodeDesc* g, std::string path, bool layout = false);
void toSvg(const GraphNode2NodeDesc* g, std::string path, bool layout = false);

GraphNode2LayoutDescTmp layout(const GraphNode2NodeDesc* g);

}  // namespace ogdf
}  // namespace external
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_EXTERNAL_OGDF_OGDF_PROXY_H_
