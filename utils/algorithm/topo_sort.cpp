/**
 * @file topo_sort.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-21 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/algorithm/topo_sort.h"

#include <unordered_map>

namespace utils {
namespace algorithm {
namespace toposort {

enum NodeState { NODE_UNVISITED, NODE_ACTIVE, NODE_VISITED };

static bool get_post_order(size_t node_idx, const GraphNode2EdgeDesc& nodes,
                           const std::unordered_map<size_t, size_t>& node_map,
                           std::vector<NodeState>& node_states,
                           std::vector<size_t>& order) {
  NodeState& node_state = node_states[node_idx];
  if (node_state == NODE_ACTIVE) {
    // cerr << "ERROR: Graph contains a cycle" << endl;
    return false;
  } else if (node_state == NODE_VISITED) {
    return true;
  } else {
    node_state = NODE_ACTIVE;
    for (auto const& input : nodes.getInput(node_idx)) {
      if (!node_map.count(input)) {
        continue;
      }
      size_t input_node_idx = node_map.at(input);
      if (!get_post_order(input_node_idx, nodes, node_map, node_states,
                          order)) {
        return false;
      }
    }
    node_state = NODE_VISITED;
    order.push_back(node_idx);
  }
  return true;
}

bool toposort(const GraphNode2EdgeDesc& nodes, std::vector<size_t>& order) {
  std::unordered_map<size_t, size_t> node_map;
  for (size_t i = 0; i < nodes.getLen(); ++i) {
    for (auto const& output : nodes.getOutput(i)) {
      if (!node_map.emplace(output, i).second) {
        // LOG_ERROR("Output name is not unique: " << output);
        return false;
      }
    }
  }
  order.reserve(nodes.getLen());
  std::vector<NodeState> node_states(nodes.getLen(), NODE_UNVISITED);
  for (size_t i = 0; i < nodes.getLen(); ++i) {
    if (!get_post_order(i, nodes, node_map, node_states, order)) {
      return false;
    }
  }
  return true;
}

}  // namespace toposort
}  // namespace algorithm
}  // namespace utils
