/**
 * @file node.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-04 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/node.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

NodeObj* NodeObj::Create(SimOnnxCtx* ctx, FakeNode_t args) {
  return new FakeNodeObj(ctx, args);
}
NodeObj* NodeObj::Create(SimOnnxCtx* ctx, NodeProtoPtr handle) {
  return new RealNodeObj(ctx, handle);
}

NodeObj* NodeObj::Create(SimOnnxCtx* ctx, ValueInfoProtoPtr handle,
                         IONodeType type) {
  if (type == kInputNode) {
    return new InputNodeObj(ctx, handle);
  }
  if (type == kOutputNode) {
    return new OutputNodeObj(ctx, handle);
  }
  LOG(FATAL) << "error type";
  return nullptr;
}

std::string RealNodeObj::getName() { return handle_->name(); }

bool RealNodeObj::setName(std::string name) {
  handle_->set_name(name);
  return true;
}

std::string RealNodeObj::getOpType() { return handle_->op_type(); }

bool RealNodeObj::setOpType(std::string op_type) {
  handle_->set_op_type(op_type);
  return true;
}

std::vector<std::string> RealNodeObj::getInputs() {
  std::vector<std::string> ret(handle_->input_size());
  for (size_t i = 0; i < ret.size(); i++) {
    ret[i] = handle_->input(i);
  }
  return ret;
}

bool RealNodeObj::setInputs(const std::vector<std::string>& inputs) {
  handle_->clear_input();
  auto& obj = *handle_->mutable_input();
  for (const auto& n : inputs) {
    *obj.Add() = n;
  }
  return true;
}

std::vector<std::string> RealNodeObj::getOutputs() {
  std::vector<std::string> ret(handle_->output_size());
  for (size_t i = 0; i < ret.size(); i++) {
    ret[i] = handle_->output(i);
  }
  return ret;
}

bool RealNodeObj::setOutputs(const std::vector<std::string>& outputs) {
  handle_->clear_output();
  auto& obj = *handle_->mutable_output();
  for (const auto& n : outputs) {
    *obj.Add() = n;
  }
  return true;
}

bool RealNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

std::string IONodeObj::getName() { return handle_->name(); }

bool IONodeObj::setName(std::string name) {
  handle_->set_name(name);
  return true;
}

std::vector<std::string> InputNodeObj::getOutputs() { return {getName()}; }

bool InputNodeObj::setOutputs(const std::vector<std::string>& outputs) {
  CHECK_EQ(outputs.size(), 1);
  return setName(outputs[0]);
}

bool InputNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

std::vector<std::string> OutputNodeObj::getInputs() { return {getName()}; }

bool OutputNodeObj::setInputs(const std::vector<std::string>& inputs) {
  CHECK_EQ(inputs.size(), 1);
  return setName(inputs[0]);
}

bool OutputNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

bool InitNodeObj::setOutputs(const std::vector<std::string>& outputs) {
  CHECK_EQ(outputs.size(), 1);
  return setName(outputs[0]);
}

bool InitNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

}  // namespace simonnx
}  // namespace utils
