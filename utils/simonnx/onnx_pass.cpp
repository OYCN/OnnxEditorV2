#include "utils/simonnx/onnx_pass.h"

#include <onnx/onnx_pb.h>

#include <map>
#include <string>

#include "utils/algorithm/topo_sort.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

bool OnnxPass::remove_useless_tensor(SimOnnxCtx* ctx) {
  // TODO(opluss): Impl
  return true;
}

bool OnnxPass::remove_useless_node(SimOnnxCtx* ctx) {
  // TODO(opluss): Impl
  return true;
}

bool OnnxPass::remove_deleted_node(SimOnnxCtx* ctx) {
  // TODO(opluss): Impl
  return true;
}

class TmpDesc : public utils::algorithm::toposort::GraphNode2EdgeDesc {
 public:
  template <typename _T>
  TmpDesc(const _T& nodes) {
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

bool OnnxPass::node_topo_sort(SimOnnxCtx* ctx) {
  auto nodes = ctx->mp_->mutable_graph()->mutable_node();
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

}  // namespace simonnx
}  // namespace utils
