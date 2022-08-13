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

#include "gui/graph/node.hpp"
#include "gui/graph/scene.h"
#include "gui/test/test.h"

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
void add_random_nodes_with_title(graph::Scene* scene, int num) {
  if (num <= 0) {
    return;
  }
  auto titles = gen_strs(num);
  auto hw = std::sqrt(num);
  std::vector<graph::Node*> nodes;
  qreal max_h = 0;
  qreal max_w = 0;
  for (const auto& title : titles) {
    auto node = scene->addNode(title.c_str(), {}, {});
    auto rect = node->boundingRect();
    if (max_h < rect.height()) {
      max_h = rect.height();
    }
    if (max_w < rect.width()) {
      max_w = rect.width();
    }
    nodes.push_back(node);
  }
  max_h *= 1.3;
  max_w *= 1.3;
  LOG(INFO) << "Gen " << nodes.size() << " Nodes";
  LOG(INFO) << "Max rect h is " << max_h;
  LOG(INFO) << "Max rect w is " << max_w;
  int w_num = 0;
  qreal now_x = 0;
  qreal now_y = 0;
  for (auto& node : nodes) {
    node->setPos(now_x, now_y);
    LOG(INFO) << "Set Random Node to (" << now_x << ", " << now_y << ")";
    w_num++;
    if (w_num > hw) {
      w_num = 0;
      now_y += max_h;
      now_x = 0;
    } else {
      now_x += max_w;
    }
  }
}
}  // namespace test

}  // namespace gui
