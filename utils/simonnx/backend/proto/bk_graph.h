/**
 * @file bk_graph.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-07 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_GRAPH_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_GRAPH_H_

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/backend/proto/bk_ctx.h"

namespace ONNX_NAMESPACE {
class GraphProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

class ProtoBackendCtx;
class ProtoBackendAttribute;

using GraphProtoPtr = ::ONNX_NAMESPACE::GraphProto*;

class ProtoBackendGraph : public ::utils::simonnx::backend::IBackendGraph {
 public:
  ProtoBackendGraph(ProtoBackendCtx* parent, GraphProtoPtr handle);
  ProtoBackendGraph(ProtoBackendAttribute* parent, GraphProtoPtr handle);
  bool destroy() override;

  const std::vector<SBackendNode>& node() const override;
  SBackendNode add_node() override;
  bool del_node(SBackendNode node) override;
  const std::vector<SBackendValueInfo>& input() const override;
  SBackendValueInfo add_input() override;
  bool del_input(SBackendValueInfo input) override;
  const std::vector<SBackendValueInfo>& output() const override;
  SBackendValueInfo add_output() override;
  bool del_output(SBackendValueInfo output) override;
  const std::vector<SBackendTensor>& initializer() const override;
  SBackendTensor add_initializer() override;
  bool del_initializer(SBackendTensor tensor) override;
  const std::vector<SBackendValueInfo>& value_info() const override;
  SBackendValueInfo add_value_info() override;
  bool del_value_info(SBackendValueInfo value_info) override;

 public:
  GraphProtoPtr getHandle() { return handle_; }
  bool remove_node_sp(IBackendNode* ptr);
  bool remove_input_sp(IBackendValueInfo* ptr);
  bool remove_output_sp(IBackendValueInfo* ptr);
  bool remove_initializer_sp(IBackendTensor* ptr);
  bool remove_value_info_sp(IBackendValueInfo* ptr);

 private:
  void initVar();
  void deinitVar();

 private:
  ProtoBackendCtx* ctx_parent_;
  ProtoBackendAttribute* attr_parent_;
  GraphProtoPtr handle_;

  std::vector<SBackendNode> node_;
  std::vector<SBackendValueInfo> input_;
  std::vector<SBackendValueInfo> output_;
  std::vector<SBackendTensor> init_;
  std::vector<SBackendValueInfo> value_info_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_GRAPH_H_
