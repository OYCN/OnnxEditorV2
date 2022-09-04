/**
 * @file test.h
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

#ifndef GUI_TEST_TEST_H_
#define GUI_TEST_TEST_H_

#include "utils/algorithm/graph_desc.h"

namespace gui {
namespace graph {
class Scene;
}

namespace test {
using GraphNode2NodeDescExtTmp =
    utils::algorithm::desc::GraphNode2NodeDescExtTmp;

GraphNode2NodeDescExtTmp gen_random_graph(int num);
}  // namespace test
}  // namespace gui

#endif  // GUI_TEST_TEST_H_
