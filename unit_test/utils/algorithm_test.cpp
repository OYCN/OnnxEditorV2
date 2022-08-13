/**
 * @file algorithm_test.cpp
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

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"
#include "utils/algorithm/layout.h"
#include "utils/algorithm/topo_sort.h"

using namespace utils::algorithm::toposort;        // NOLINT
using namespace utils::algorithm::layout;          // NOLINT
using namespace utils::algorithm::external::ogdf;  // NOLINT

class T : public GraphNode2EdgeDesc {
 public:
  T() {
    /*
    --|-----------
    A | 0 -> 1
    --|-----------
    B | 1 ->|-> 2
      |     |-> 3
    --|-----------
    C | 0 ->|-> 4
      | 3 ->|
    --|-----------
    */
    // C, B, A
    inputs_ = {{0, 3}, {1}, {0}};
    outputs_ = {{4}, {2, 3}, {1}};
  }

 public:
  size_t getLen() const override { return 3; }

  std::vector<size_t> getOutput(size_t i) const override { return outputs_[i]; }

  std::vector<size_t> getInput(size_t i) const override { return inputs_[i]; }

 private:
  std::vector<std::vector<size_t>> inputs_;
  std::vector<std::vector<size_t>> outputs_;
};

TEST(algorithm, topo_sort) {
  T t;
  std::vector<size_t> order;
  auto ret = toposort(t, order);
  ASSERT_EQ(order.size(), t.getLen());
  for (size_t i = 0; i < order.size(); i++) {
    EXPECT_EQ(order[i], order.size() - i - 1) << i;
  }
  ASSERT_TRUE(ret);
}

TEST(algorithm, layout) {
  auto g = genRandomGraph(10, 10);
  auto ret = Layout::layout(g.get(), LayoutAlgorithm_t::kOGDF);
  EXPECT_EQ(ret.getLen(), g->getLen());
}
