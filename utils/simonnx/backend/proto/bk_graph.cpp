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
    LOG(INFO) << "delete Graph from ctx success";
    return true;
  }
  if (attr_parent_ != nullptr) {
    // TODO(oPluss): impl
    LOG(ERROR) << "delete Graph from attr not impl";
    return false;
  }
  LOG(ERROR) << "delete Graph failed";
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
bool ProtoBackendGraph::del_output(SBackendValueInfo output) {
  CHECK_HANDLE_DEL(false);
  return output->destroy();
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

bool ProtoBackendGraph::remove_node_sp(IBackendNode* ptr) {
  for (size_t i = 0; i < node_.size(); i++) {
    if (node_[i].get() == ptr) {
      std::swap(node_[i], node_.back());
      node_.pop_back();
      return true;
    }
  }
  return false;
}
bool ProtoBackendGraph::remove_input_sp(IBackendValueInfo* ptr) {
  for (size_t i = 0; i < input_.size(); i++) {
    if (input_[i].get() == ptr) {
      std::swap(input_[i], input_.back());
      input_.pop_back();
      return true;
    }
  }
  return false;
}
bool ProtoBackendGraph::remove_output_sp(IBackendValueInfo* ptr) {
  for (size_t i = 0; i < output_.size(); i++) {
    if (output_[i].get() == ptr) {
      std::swap(output_[i], output_.back());
      output_.pop_back();
      return true;
    }
  }
  return false;
}
bool ProtoBackendGraph::remove_initializer_sp(IBackendTensor* ptr) {
  for (size_t i = 0; i < init_.size(); i++) {
    if (init_[i].get() == ptr) {
      std::swap(init_[i], init_.back());
      init_.pop_back();
      return true;
    }
  }
  return false;
}
bool ProtoBackendGraph::remove_value_info_sp(IBackendValueInfo* ptr) {
  for (size_t i = 0; i < value_info_.size(); i++) {
    if (value_info_[i].get() == ptr) {
      std::swap(value_info_[i], value_info_.back());
      value_info_.pop_back();
      return true;
    }
  }
  return false;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
