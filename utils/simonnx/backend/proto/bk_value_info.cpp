/**
 * @file bk_tensor.cpp
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
#include "utils/simonnx/backend/proto/bk_value_info.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_graph.h"
#include "utils/simonnx/backend/proto/helper.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

ProtoBackendValueInfo::ProtoBackendValueInfo(ProtoBackendGraph* parent,
                                             ValueInfoProtoPtr handle)
    : parent_(parent), handle_(handle) {}

bool ProtoBackendValueInfo::destroy() {
  LOG(INFO) << "delete ValueInfoProtoPtr";
  auto ins = parent_->getHandle()->mutable_input();
  auto outs = parent_->getHandle()->mutable_output();
  auto vals = parent_->getHandle()->mutable_value_info();
  if ((delFromRepeatProto(ins, handle_) && parent_->remove_input_sp(this)) ||
      (delFromRepeatProto(outs, handle_) && parent_->remove_output_sp(this)) ||
      (delFromRepeatProto(vals, handle_) &&
       parent_->remove_value_info_sp(this))) {
    handle_ = nullptr;
    LOG(INFO) << "delete ValueInfo success";
    return true;
  } else {
    LOG(ERROR) << "delete ValueInfo failed @" << handle_;
    return false;
  }
}

std::string ProtoBackendValueInfo::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}
bool ProtoBackendValueInfo::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}
std::string ProtoBackendValueInfo::type() const {
  CHECK_HANDLE_DEL("");
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
  auto t = static_cast<ONNX_NAMESPACE::TensorProto::DataType>(
      tensor_type.elem_type());
  return ONNX_NAMESPACE::TensorProto::DataType_Name(t);
}
bool ProtoBackendValueInfo::set_type(const std::string& type) {
  CHECK_HANDLE_DEL(false);
  ONNX_NAMESPACE::TensorProto::DataType dt;
  if (!ONNX_NAMESPACE::TensorProto::DataType_Parse(type, &dt)) {
    return false;
  }
  handle_->mutable_type()->mutable_tensor_type()->set_elem_type(dt);
  return true;
}
std::vector<int64_t> ProtoBackendValueInfo::dim() const {
  CHECK_HANDLE_DEL({});
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
bool ProtoBackendValueInfo::set_dim(const std::vector<int64_t>& dim) {
  CHECK_HANDLE_DEL(false);
  auto type = handle_->mutable_type();
  auto tensor_type = type->mutable_tensor_type();
  auto shape = tensor_type->mutable_shape();
  shape->clear_dim();
  auto dims = shape->mutable_dim();
  for (auto& in_dim : dim) {
    auto d = dims->Add();
    d->set_dim_value(in_dim);
  }
  return true;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
