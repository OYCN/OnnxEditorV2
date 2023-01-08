/**
 * @file bk_node.cpp
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

#include "utils/simonnx/backend/proto/bk_node.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_graph.h"
#include "utils/simonnx/backend/proto/helper.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

ProtoBackendNode::ProtoBackendNode(ProtoBackendGraph* parent,
                                   NodeProtoPtr handle)
    : parent_(parent), handle_(handle) {}

bool ProtoBackendNode::destroy() {
  VLOG(1) << "delete NodeProtoPtr";
  auto nodes = parent_->getHandle()->mutable_node();
  if (delFromRepeatProto(nodes, handle_)) {
    handle_ = nullptr;
    attr_.clear();
    return true;
  } else {
    LOG(ERROR) << "delete NodeProtoPtr failed @" << handle_;
    return false;
  }
}

std::string ProtoBackendNode::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}
bool ProtoBackendNode::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}
std::string ProtoBackendNode::op_type() const {
  CHECK_HANDLE_DEL("");
  return handle_->op_type();
}
bool ProtoBackendNode::set_op_type(const std::string& op_type) {
  CHECK_HANDLE_DEL(false);
  handle_->set_op_type(op_type);
  return true;
}
std::vector<std::string> ProtoBackendNode::input() const {
  CHECK_HANDLE_DEL({});
  std::vector<std::string> ret(handle_->input_size());
  for (size_t i = 0; i < ret.size(); i++) {
    ret[i] = handle_->input(i);
  }
  return ret;
}
bool ProtoBackendNode::set_input(const std::vector<std::string>& input) {
  CHECK_HANDLE_DEL(false);
  handle_->clear_input();
  auto& obj = *handle_->mutable_input();
  for (const auto& n : input) {
    *obj.Add() = n;
  }
  return true;
}
std::vector<std::string> ProtoBackendNode::output() const {
  CHECK_HANDLE_DEL({});
  std::vector<std::string> ret(handle_->output_size());
  for (size_t i = 0; i < ret.size(); i++) {
    ret[i] = handle_->output(i);
  }
  return ret;
}
bool ProtoBackendNode::set_output(const std::vector<std::string>& output) {
  CHECK_HANDLE_DEL(false);
  handle_->clear_output();
  auto& obj = *handle_->mutable_output();
  for (const auto& n : output) {
    *obj.Add() = n;
  }
  return true;
}
const std::vector<SBackendAttribute>& ProtoBackendNode::attribute() const {
  CHECK_HANDLE_DEL(attr_);
  return attr_;
}
SBackendAttribute ProtoBackendNode::add_attribute() {
  CHECK_HANDLE_DEL(nullptr);
  // TODO(oPluss): impl
  return nullptr;
}
bool ProtoBackendNode::del_attribute(SBackendAttribute attr) {
  CHECK_HANDLE_DEL(false);
  // TODO(oPluss): impl
  return false;
}

ProtoBackendAttribute::ProtoBackendAttribute(ProtoBackendNode* parent,
                                             AttributeProtoPtr handle)
    : parent_(parent), handle_(handle) {}

std::string ProtoBackendAttribute::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}

bool ProtoBackendAttribute::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
