/**
 * @file bk_graph.cpp
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
#include "utils/simonnx/backend/proto/bk_graph.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_ctx.h"
#include "utils/simonnx/backend/proto/bk_node.h"
#include "utils/simonnx/backend/proto/bk_tensor.h"
#include "utils/simonnx/backend/proto/bk_value_info.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

ProtoBackendGraph::ProtoBackendGraph(ProtoBackendCtx* parent,
                                     GraphProtoPtr handle)
    : parent_(parent), handle_(handle) {
  auto nodes = handle_->mutable_node();
  for (size_t i = 0; i < nodes->size(); i++) {
    node_.emplace_back(
        std::make_shared<ProtoBackendNode>(this, nodes->Mutable(i)));
  }
  auto inputs = handle_->mutable_input();
  for (size_t i = 0; i < inputs->size(); i++) {
    input_.emplace_back(
        std::make_shared<ProtoBackendValueInfo>(this, inputs->Mutable(i)));
  }
  auto outputs = handle_->mutable_output();
  for (size_t i = 0; i < outputs->size(); i++) {
    output_.emplace_back(
        std::make_shared<ProtoBackendValueInfo>(this, outputs->Mutable(i)));
  }
  auto inits = handle_->mutable_initializer();
  for (size_t i = 0; i < inits->size(); i++) {
    init_.emplace_back(
        std::make_shared<ProtoBackendTensor>(this, inits->Mutable(i)));
  }
  auto value_infoes = handle_->mutable_value_info();
  for (size_t i = 0; i < value_infoes->size(); i++) {
    value_info_.emplace_back(std::make_shared<ProtoBackendValueInfo>(
        this, value_infoes->Mutable(i)));
  }
}
bool ProtoBackendGraph::destroy() {
  // TODO(oPluss): impl
  return false;
}

const std::vector<SBackendNode>& ProtoBackendGraph::node() const {
  return node_;
}
SBackendNode ProtoBackendGraph::add_node() {
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendGraph::del_node(SBackendNode node) {
  // TODO(oPluss): impl
  return false;
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::input() const {
  return input_;
}
SBackendValueInfo ProtoBackendGraph::add_input() {
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendGraph::del_input(SBackendValueInfo input) {
  // TODO(oPluss): impl
  return false;
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::output() const {
  return output_;
}
SBackendValueInfo ProtoBackendGraph::add_output() {
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendGraph::del_output(SBackendValueInfo value_info) {
  // TODO(oPluss): impl
  return false;
}
const std::vector<SBackendTensor>& ProtoBackendGraph::initializer() const {
  // TODO(oPluss): impl
  return init_;
}
SBackendTensor ProtoBackendGraph::add_initializer() {
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendGraph::del_initializer(SBackendTensor tensor) {
  // TODO(oPluss): impl
  return false;
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::value_info() const {
  // TODO(oPluss): impl
  return value_info_;
}
SBackendValueInfo ProtoBackendGraph::add_value_info() {
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendGraph::del_value_info(SBackendValueInfo value_info) {
  // TODO(oPluss): impl
  return false;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
