/**
 * @file onnx_pass.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-26 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/backend/proto/onnx_pass.h"

#include <onnx/onnx_pb.h>

#include <map>
#include <string>

#include "utils/algorithm/topo_sort.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

bool OnnxPass::remove_useless_node(ModelProtoPtr mp) {
  // TODO(opluss): Impl
  return true;
}

bool OnnxPass::remove_useless_tensor(ModelProtoPtr mp) {
  // TODO(opluss): Impl
  return true;
}

class TmpDesc : public utils::algorithm::toposort::GraphNode2EdgeDesc {
 public:
  template <typename _T>
  explicit TmpDesc(const _T& nodes) {
    n2i.resize(nodes.size());
    n2o.resize(nodes.size());
    size_t mi = 0;
    std::map<std::string, size_t> m;
    for (size_t i = 0; i < nodes.size(); i++) {
      for (const auto& v : nodes[i].input()) {
        if (m.count(v) == 0) {
          m[v] = mi++;
        }
        n2i[i].emplace_back(m.at(v));
      }
      for (const auto& v : nodes[i].output()) {
        if (m.count(v) == 0) {
          m[v] = mi++;
        }
        n2o[i].emplace_back(m.at(v));
      }
    }
  }
  size_t getLen() const override { return n2i.size(); }
  std::vector<size_t> getOutput(size_t i) const override { return n2o[i]; }
  std::vector<size_t> getInput(size_t i) const override { return n2i[i]; }

 private:
  std::vector<std::vector<size_t>> n2i;
  std::vector<std::vector<size_t>> n2o;
};

bool OnnxPass::node_topo_sort(ModelProtoPtr mp) {
  auto nodes = mp->mutable_graph()->mutable_node();
  TmpDesc td(*nodes);
  std::vector<size_t> idxes;
  if (!utils::algorithm::toposort::toposort(td, idxes)) {
    return false;
  }
  for (size_t i = 0; i < idxes.size();) {
    if (idxes[i] != i) {
      nodes->SwapElements(i, idxes[i]);
      std::swap(i, idxes[i]);
    } else {
      i++;
    }
  }
  return true;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
