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
#include "utils/simonnx/backend/proto/helper.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

ProtoBackendGraph::ProtoBackendGraph(ProtoBackendCtx* parent,
                                     GraphProtoPtr handle)
    : ctx_parent_(parent), attr_parent_(nullptr), handle_(handle) {
  initVar();
}
ProtoBackendGraph::ProtoBackendGraph(ProtoBackendAttribute* parent,
                                     GraphProtoPtr handle)
    : ctx_parent_(nullptr), attr_parent_(parent), handle_(handle) {
  initVar();
}

void ProtoBackendGraph::initVar() {
  CHECK_NE(ctx_parent_ == nullptr, attr_parent_ == nullptr);
  CHECK_NOTNULL(handle_);
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
void ProtoBackendGraph::deinitVar() {
  node_.clear();
  input_.clear();
  output_.clear();
  init_.clear();
  value_info_.clear();
}
bool ProtoBackendGraph::destroy() {
  if (ctx_parent_ != nullptr) {
    ctx_parent_->getHandle()->clear_graph();
    handle_ = nullptr;
    deinitVar();
    return true;
  }
  if (attr_parent_ != nullptr) {
    // TODO(oPluss): impl
    return false;
  }
  return false;
}

const std::vector<SBackendNode>& ProtoBackendGraph::node() const {
  CHECK_HANDLE_DEL(node_);
  return node_;
}
SBackendNode ProtoBackendGraph::add_node() {
  CHECK_HANDLE_DEL(nullptr);
  node_.emplace_back(
      std::make_shared<ProtoBackendNode>(this, handle_->mutable_node()->Add()));
  return node_.back();
}
bool ProtoBackendGraph::del_node(SBackendNode node) {
  CHECK_HANDLE_DEL(false);
  return node->destroy();
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::input() const {
  CHECK_HANDLE_DEL(input_);
  return input_;
}
SBackendValueInfo ProtoBackendGraph::add_input() {
  CHECK_HANDLE_DEL(nullptr);
  input_.emplace_back(std::make_shared<ProtoBackendValueInfo>(
      this, handle_->mutable_input()->Add()));
  return input_.back();
}
bool ProtoBackendGraph::del_input(SBackendValueInfo input) {
  CHECK_HANDLE_DEL(false);
  return input->destroy();
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::output() const {
  CHECK_HANDLE_DEL(output_);
  return output_;
}
SBackendValueInfo ProtoBackendGraph::add_output() {
  CHECK_HANDLE_DEL(nullptr);
  output_.emplace_back(std::make_shared<ProtoBackendValueInfo>(
      this, handle_->mutable_output()->Add()));
  return output_.back();
}
bool ProtoBackendGraph::del_output(SBackendValueInfo value_info) {
  CHECK_HANDLE_DEL(false);
  return value_info->destroy();
}
const std::vector<SBackendTensor>& ProtoBackendGraph::initializer() const {
  CHECK_HANDLE_DEL(init_);
  return init_;
}
SBackendTensor ProtoBackendGraph::add_initializer() {
  CHECK_HANDLE_DEL(nullptr);
  init_.emplace_back(std::make_shared<ProtoBackendTensor>(
      this, handle_->mutable_initializer()->Add()));
  return init_.back();
}
bool ProtoBackendGraph::del_initializer(SBackendTensor tensor) {
  CHECK_HANDLE_DEL(false);
  return tensor->destroy();
}
const std::vector<SBackendValueInfo>& ProtoBackendGraph::value_info() const {
  CHECK_HANDLE_DEL(value_info_);
  return value_info_;
}
SBackendValueInfo ProtoBackendGraph::add_value_info() {
  CHECK_HANDLE_DEL(nullptr);
  value_info_.emplace_back(std::make_shared<ProtoBackendValueInfo>(
      this, handle_->mutable_value_info()->Add()));
  return value_info_.back();
}
bool ProtoBackendGraph::del_value_info(SBackendValueInfo value_info) {
  CHECK_HANDLE_DEL(false);
  return value_info->destroy();
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
