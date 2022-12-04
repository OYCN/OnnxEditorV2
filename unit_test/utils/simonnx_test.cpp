/**
 * @file parser_test.cpp
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

#include <gtest/gtest.h>

#include "utils/simonnx/onnx_help.h"

TEST(simonnx, gen_random_graph) {
  auto g = utils::simonnx::gen_random_graph(10);
}
