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

// #include <magic_enum.hpp>

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

NodeObj* NodeObj::Create(SimOnnxCtx* ctx, FakeNode_t args) {
  return new FakeNodeObj(ctx, args);
}
NodeObj* NodeObj::Create(SimOnnxCtx* ctx, SBackendNode handle_) {
  return new RealNodeObj(ctx, handle_);
}

NodeObj* NodeObj::Create(SimOnnxCtx* ctx, SBackendValueInfo handle_,
                         IONodeType type) {
  if (type == kInputNode) {
    return new InputNodeObj(ctx, handle_);
  }
  if (type == kOutputNode) {
    return new OutputNodeObj(ctx, handle_);
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

std::vector<std::string> RealNodeObj::getInputs() { return handle_->input(); }

bool RealNodeObj::setInputs(const std::vector<std::string>& inputs) {
  return handle_->set_input(inputs);
}

std::vector<std::string> RealNodeObj::getOutputs() { return handle_->output(); }

bool RealNodeObj::setOutputs(const std::vector<std::string>& outputs) {
  return handle_->set_output(outputs);
}

bool RealNodeObj::destroyHandle() { return handle_->destroy(); }

std::string IONodeObj::getName() { return handle_->name(); }

bool IONodeObj::setName(std::string name) { return handle_->set_name(name); }

std::vector<int64_t> IONodeObj::getDim() { return handle_->dim(); }

bool IONodeObj::setDim(const std::vector<int64_t>& in_dims) {
  return handle_->set_dim(in_dims);
}

std::string IONodeObj::getDataType() { return handle_->type(); }

bool IONodeObj::setDataType(const std::string& datatype) {
  return handle_->set_type(datatype);
}

std::vector<std::string> InputNodeObj::getOutputs() { return {getName()}; }

// bool InputNodeObj::setOutputs(const std::vector<std::string>& outputs) {
//   CHECK_EQ(outputs.size(), 1);
//   return setName(outputs[0]);
// }

bool InputNodeObj::destroyHandle() { return handle_->destroy(); }

std::vector<std::string> OutputNodeObj::getInputs() { return {getName()}; }

// bool OutputNodeObj::setInputs(const std::vector<std::string>& inputs) {
//   CHECK_EQ(inputs.size(), 1);
//   return setName(inputs[0]);
// }

bool OutputNodeObj::destroyHandle() { return handle_->destroy(); }

// bool InitNodeObj::setOutputs(const std::vector<std::string>& outputs) {
//   CHECK_EQ(outputs.size(), 1);
//   return setName(outputs[0]);
// }

bool InitNodeObj::destroyHandle() { return handle_->destroy(); }

}  // namespace simonnx
}  // namespace utils
