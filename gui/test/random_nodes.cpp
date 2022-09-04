/**
 * @file random_nodes.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-20 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include <glog/logging.h>

#include <cmath>
#include <sstream>
#include <string>

#include "gui/graph/node.h"
#include "gui/graph/scene.h"
#include "gui/test/test.h"
#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

using namespace utils::algorithm::external::ogdf;  // NOLINT

namespace gui {
namespace test {
std::vector<std::string> gen_strs(int num) {
  std::vector<std::string> out;
  if (num <= 0) {
    return out;
  }
  std::vector<char> char_list;
  // for (char c = 'A'; c <= 'Z'; c++) {
  //   char_list.push_back(c);
  // }
  for (char c = 'a'; c <= 'z'; c++) {
    char_list.push_back(c);
  }
  for (char c = '0'; c <= '9'; c++) {
    char_list.push_back(c);
  }
  for (int i = 0; i < num; i++) {
    std::stringstream ss;
    int local_i = i;
    while (local_i >= char_list.size()) {
      ss << char_list[local_i % char_list.size()];
      local_i /= char_list.size();
    }
    ss << char_list[local_i];
    out.push_back(ss.str());
    out.back().reserve();
  }
  return out;
}

GraphNode2NodeDescExtTmp gen_random_graph(int num) {
  auto g = genRandomGraph(num, num);

  std::vector<std::vector<std::string>> node2name(g.getLen());
  std::vector<std::vector<std::pair<std::string, std::string>>> node2attrs(
      g.getLen());
  for (size_t i = 0; i < g.getLen(); i++) {
    node2name[i] = {std::to_string(i), std::to_string(i)};
    node2attrs[i] = {};
  }

  size_t edge_count = 0;
  std::map<std::pair<size_t, size_t>, std::string> edge2name;
  for (size_t i = 0; i < g.getLen(); i++) {
    for (auto j : g.getOutput(i)) {
      edge2name[{i, j}] = std::to_string(edge_count++);
    }
  }

  return GraphNode2NodeDescExtTmp(g, node2name, node2attrs, edge2name);
}

}  // namespace test

}  // namespace gui
