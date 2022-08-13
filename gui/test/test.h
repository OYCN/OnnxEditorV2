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

namespace gui {
namespace graph {
class Scene;
}

namespace test {
void add_random_nodes_with_title(graph::Scene* scene, int num);
}
}  // namespace gui

#endif  // GUI_TEST_TEST_H_
