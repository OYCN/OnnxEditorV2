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

#include "utils/simonnx/backend/proto/bk_tensor.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_graph.h"
#include "utils/simonnx/backend/proto/helper.h"
namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

std::string getType(const TensorProtoPtr handle) {
  std::string ret;
  if (!handle->has_data_type()) {
    return ret;
  }
  auto data_type = handle->data_type();
  CHECK(ONNX_NAMESPACE::TensorProto::DataType_IsValid(data_type));
  auto t = static_cast<ONNX_NAMESPACE::TensorProto::DataType>(data_type);
  return ONNX_NAMESPACE::TensorProto::DataType_Name(t);
}
std::vector<int64_t> getDim(const TensorProtoPtr handle) {
  // TODO(oPluss): impl
  return {};
}
Buff getData(const TensorProtoPtr handle) {
  // TODO(oPluss): impl
  return {};
}

ProtoBackendTensor::ProtoBackendTensor(ProtoBackendGraph* parent,
                                       TensorProtoPtr handle)
    : graph_parent_(parent), attr_parent_(nullptr), handle_(handle) {}

ProtoBackendTensor::ProtoBackendTensor(ProtoBackendAttribute* parent,
                                       TensorProtoPtr handle)
    : graph_parent_(nullptr), attr_parent_(parent), handle_(handle) {}

bool ProtoBackendTensor::destroy() {
  LOG(INFO) << "delete TensorProtoPtr";
  if (graph_parent_ != nullptr) {
    auto inits = graph_parent_->getHandle()->mutable_initializer();
    if (delFromRepeatProto(inits, handle_) &&
        graph_parent_->remove_initializer_sp(this)) {
      handle_ = nullptr;
      LOG(INFO) << "delete Tensor from graph success";
      return true;
    } else {
      LOG(ERROR) << "delete Tensor from graph failed @" << handle_;
      return false;
    }
  } else if (attr_parent_ != nullptr) {
    // TODO(oPluss): Impl
    LOG(ERROR) << "delete Tensor from attr not impl";
    return false;
  }
  LOG(ERROR) << "delete Tensor failed";
  return false;
}

std::string ProtoBackendTensor::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}
bool ProtoBackendTensor::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}
std::string ProtoBackendTensor::type() const {
  CHECK_HANDLE_DEL("");
  return getType(handle_);
}
bool ProtoBackendTensor::set_type(const std::string& type) {
  CHECK_HANDLE_DEL(false);
  ONNX_NAMESPACE::TensorProto::DataType dt;
  if (!ONNX_NAMESPACE::TensorProto::DataType_Parse(type, &dt)) {
    return false;
  }
  handle_->set_data_type(dt);
  return true;
}
std::vector<int64_t> ProtoBackendTensor::dim() const {
  CHECK_HANDLE_DEL({});
  return getDim(handle_);
}
bool ProtoBackendTensor::set_dim(const std::vector<int64_t>& dim) {
  CHECK_HANDLE_DEL(false);
  // TODO(oPluss): impl
  return false;
}
Buff ProtoBackendTensor::data() const {
  CHECK_HANDLE_DEL({});
  // TODO(oPluss): impl
  return {};
}
bool ProtoBackendTensor::set_data(Buff buff) {
  CHECK_HANDLE_DEL(false);
  // TODO(oPluss): impl
  return false;
}

TmpTensor::TmpTensor(const TensorProtoPtr handle) {
  name_ = handle->name();
  type_ = getType(handle);
  dim_ = getDim(handle);
  buff_ = getData(handle);
}
bool TmpTensor::destroy() { return false; }
std::string TmpTensor::name() const { return name_; }
bool TmpTensor::set_name(const std::string& name) {
  name_ = name;
  return true;
}
std::string TmpTensor::type() const { return type_; }
bool TmpTensor::set_type(const std::string& type) {
  type_ = type;
  return true;
}
std::vector<int64_t> TmpTensor::dim() const { return dim_; }
bool TmpTensor::set_dim(const std::vector<int64_t>& dim) {
  dim_ = dim;
  return true;
}
Buff TmpTensor::data() const { return buff_; }
bool TmpTensor::set_data(Buff buff) {
  buff_ = buff;
  return true;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
