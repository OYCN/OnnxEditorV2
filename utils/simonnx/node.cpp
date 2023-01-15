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

#include <string>

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

template <typename _T>
using ArrayStr = utils::algorithm::parse::ArrayStr<_T>;

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

std::string RealNodeObj::getName() const { return handle_->name(); }

bool RealNodeObj::setName(std::string name) {
  handle_->set_name(name);
  return true;
}

std::string RealNodeObj::getOpType() const { return handle_->op_type(); }

bool RealNodeObj::setOpType(std::string op_type) {
  handle_->set_op_type(op_type);
  return true;
}

std::vector<std::string> RealNodeObj::getInputs() const {
  return handle_->input();
}

bool RealNodeObj::setInputs(const std::vector<std::string>& inputs) {
  return handle_->set_input(inputs);
}

std::vector<std::string> RealNodeObj::getOutputs() const {
  return handle_->output();
}

bool RealNodeObj::setOutputs(const std::vector<std::string>& outputs) {
  return handle_->set_output(outputs);
}

std::string readValueFromAttr(SBackendAttribute src) {
  auto type = src->type();
  if (type == "FLOAT") {
    return std::to_string(src->f());
  } else if (type == "INT") {
    return std::to_string(src->i());
  } else if (type == "FLOATS") {
    return ArrayStr<float>::Array2Str(src->floats());
  } else if (type == "INTS") {
    return ArrayStr<int>::Array2Str(src->ints());
  } else {
    LOG(ERROR) << "unhandled type: " << type;
    return "";
  }
}

std::vector<NodeAttr_t> RealNodeObj::getAttrs() const {
  std::vector<NodeAttr_t> ret;
  auto attrs = handle_->attribute();
  for (auto attr : attrs) {
    ret.emplace_back();
    auto& r = ret.back();
    r.key = attr->name();
    r.type = attr->type();
    r.value = readValueFromAttr(attr);
  }
  return ret;
}

bool apply2Attr(SBackendAttribute dst, const NodeAttr_t& src) {
  auto& type = src.type;
  bool ret = true;
  if (type == "FLOAT") {
    ret = dst->set_f(std::stof(src.value)) && ret;
  } else if (type == "INT") {
    ret = dst->set_i(std::stoi(src.value)) && ret;
  } else if (type == "STRING") {
    ret = dst->set_s(src.value) && ret;
  } else if (type == "FLOATS") {
    ret = ArrayStr<float>::StrValid(src.value) && ret;
    ret = ret && dst->set_floats(ArrayStr<float>::Str2Array(src.value));
  } else if (type == "INTS") {
    ret = ArrayStr<int>::StrValid(src.value) && ret;
    ret = ret && dst->set_ints(ArrayStr<int>::Str2Array(src.value));
  } else {
    LOG(ERROR) << "unhandled type: " << type;
    return "";
  }
  if (!ret) {
    return false;
  }
  CHECK_EQ(dst->type(), src.type);
  dst->set_name(src.key);
  return true;
}

bool RealNodeObj::setAttrs(const std::vector<NodeAttr_t>& in_attrs) {
  auto attrs = handle_->attribute();
  size_t min = std::min(in_attrs.size(), attrs.size());
  bool ret = true;
  for (size_t i = 0; i < min; i++) {
    ret = apply2Attr(attrs[i], in_attrs[i]) && ret;
  }
  for (size_t i = attrs.size(); i < in_attrs.size(); i++) {
    auto attr = handle_->add_attribute();
    CHECK_NOTNULL(attr.get());
    ret = apply2Attr(attr, in_attrs[i]) && ret;
  }
  for (size_t i = in_attrs.size(); i < attrs.size(); i++) {
    ret = handle_->del_attribute(attrs[i]) && ret;
  }
  return ret;
}

bool RealNodeObj::destroyHandle() { return handle_->destroy(); }

std::string IONodeObj::getName() const { return handle_->name(); }

bool IONodeObj::setName(std::string name) { return handle_->set_name(name); }

DimStr IONodeObj::getDim() const { return DimStr(handle_->dim()); }

bool IONodeObj::setDim(const DimStr& dim) {
  if (!dim.valid()) {
    return false;
  }
  return handle_->set_dim(dim.getArray());
}

std::string IONodeObj::getDataType() const { return handle_->type(); }

bool IONodeObj::setDataType(const std::string& datatype) {
  return handle_->set_type(datatype);
}

std::vector<std::string> InputNodeObj::getOutputs() const {
  return {getName()};
}

// bool InputNodeObj::setOutputs(const std::vector<std::string>& outputs) {
//   CHECK_EQ(outputs.size(), 1);
//   return setName(outputs[0]);
// }

bool InputNodeObj::destroyHandle() { return handle_->destroy(); }

std::vector<std::string> OutputNodeObj::getInputs() const {
  return {getName()};
}

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
