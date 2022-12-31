/**
 * @file graphviz_proxy.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-30 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_ALGORITHM_EXTERNAL_GRAPHVIZ_GRAPHVIZ_PROXY_H_
#define UTILS_ALGORITHM_EXTERNAL_GRAPHVIZ_GRAPHVIZ_PROXY_H_

#include "utils/algorithm/graph_desc.h"

namespace utils {
namespace algorithm {
namespace external {
namespace graphviz {

using GraphNode2NodeDesc = desc::GraphNode2NodeDesc;
using GraphNode2NodeDescTmp = desc::GraphNode2NodeDescTmp;
using GraphNode2LayoutDescTmp = desc::GraphNode2LayoutDescTmp;

GraphNode2LayoutDescTmp layout(const GraphNode2NodeDesc* g);
void layout2dot(const GraphNode2NodeDesc* gd, std::string path);
void layout2svg(const GraphNode2NodeDesc* gd, std::string path);

}  // namespace graphviz
}  // namespace external
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_EXTERNAL_GRAPHVIZ_GRAPHVIZ_PROXY_H_
