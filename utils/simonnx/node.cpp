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

// #include <magic_enum.hpp>

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

std::vector<int64_t> IONodeObj::getDim() {
  std::vector<int64_t> ret;
  if (!handle_->has_type()) {
    return ret;
  }
  auto& type = handle_->type();
  if (!type.has_tensor_type()) {
    return ret;
  }
  auto& tensor_type = type.tensor_type();
  if (!tensor_type.has_shape()) {
    return ret;
  }
  auto& shape = tensor_type.shape();
  auto& dims = shape.dim();
  for (auto& dim : dims) {
    CHECK_EQ(ONNX_NAMESPACE::TensorShapeProto::Dimension::kDimValue,
             dim.value_case());
    ret.emplace_back(dim.dim_value());
  }
  return ret;
}

bool IONodeObj::setDim(const std::vector<int64_t>& in_dims) {
  auto type = handle_->mutable_type();
  auto tensor_type = type->mutable_tensor_type();
  auto shape = tensor_type->mutable_shape();
  shape->clear_dim();
  auto dims = shape->mutable_dim();
  for (auto& in_dim : in_dims) {
    auto d = dims->Add();
    d->set_dim_value(in_dim);
  }
  return true;
}

std::string IONodeObj::getDataType() {
  std::string ret;
  if (!handle_->has_type()) {
    return ret;
  }
  auto& type = handle_->type();
  if (!type.has_tensor_type()) {
    return ret;
  }
  auto& tensor_type = type.tensor_type();
  if (!tensor_type.has_elem_type()) {
    return ret;
  }
  CHECK(ONNX_NAMESPACE::TensorProto::DataType_IsValid(tensor_type.elem_type()));
  auto t = static_cast<ONNX_NAMESPACE::TensorProto::DataType>(tensor_type.elem_type());
  return ONNX_NAMESPACE::TensorProto::DataType_Name(t);
  // auto t = magic_enum::enum_cast<ONNX_NAMESPACE::TensorProto_DataType>(
  //     static_cast<int>(tensor_type.elem_type()));
  // if (!t.has_value()) {
  //   return ret;
  // }
  // return magic_enum::enum_name(t.value());
}

bool IONodeObj::setDataType(const std::string& datatype) {
  ONNX_NAMESPACE::TensorProto::DataType dt;
  if (!ONNX_NAMESPACE::TensorProto::DataType_Parse(datatype, &dt)) {
    return false;
  }
  handle_->mutable_type()->mutable_tensor_type()->set_elem_type(dt);
  return true;
  // auto dt = magic_enum::enum_cast<ONNX_NAMESPACE::TensorProto_DataType>(datatype);
  // if (!dt.has_value()) {
  //   return false;
  // }
  // handle_->mutable_type()->mutable_tensor_type()->set_elem_type(
  //     magic_enum::enum_integer(dt));
  // return true;
}

std::vector<std::string> InputNodeObj::getOutputs() { return {getName()}; }

// bool InputNodeObj::setOutputs(const std::vector<std::string>& outputs) {
//   CHECK_EQ(outputs.size(), 1);
//   return setName(outputs[0]);
// }

bool InputNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

std::vector<std::string> OutputNodeObj::getInputs() { return {getName()}; }

// bool OutputNodeObj::setInputs(const std::vector<std::string>& inputs) {
//   CHECK_EQ(inputs.size(), 1);
//   return setName(inputs[0]);
// }

bool OutputNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

// bool InitNodeObj::setOutputs(const std::vector<std::string>& outputs) {
//   CHECK_EQ(outputs.size(), 1);
//   return setName(outputs[0]);
// }

bool InitNodeObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

}  // namespace simonnx
}  // namespace utils
