/**
 * @file algorithm.h
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

#ifndef UTILS_ALGORITHM_TOPO_SORT_H_
#define UTILS_ALGORITHM_TOPO_SORT_H_

#include <vector>
#include "utils/algorithm/graph_desc.h"

namespace utils {
namespace algorithm {
namespace toposort {

using GraphNode2EdgeDesc = desc::GraphNode2EdgeDesc;

bool toposort(const GraphNode2EdgeDesc& nodes, std::vector<size_t>& order);

}  // namespace toposort
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_TOPO_SORT_H_
