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

ProtoBackendTensor::ProtoBackendTensor(ProtoBackendGraph* parent,
                                       TensorProtoPtr handle)
    : parent_(parent), handle_(handle) {}

bool ProtoBackendTensor::destroy() {
  LOG(INFO) << "delete TensorProtoPtr";
  auto inits = parent_->getHandle()->mutable_initializer();
  if (delFromRepeatProto(inits, handle_)) {
    return true;
  } else {
    LOG(ERROR) << "delete TensorProtoPtr failed @" << handle_;
    return false;
  }
}

std::string ProtoBackendTensor::name() const { return handle_->name(); }
bool ProtoBackendTensor::set_name(const std::string& name) {
  handle_->set_name(name);
  return true;
}
std::string ProtoBackendTensor::type() const {
  std::string ret;
  if (!handle_->has_data_type()) {
    return ret;
  }
  auto data_type = handle_->data_type();
  CHECK(ONNX_NAMESPACE::TensorProto::DataType_IsValid(data_type));
  auto t = static_cast<ONNX_NAMESPACE::TensorProto::DataType>(data_type);
  return ONNX_NAMESPACE::TensorProto::DataType_Name(t);
}
bool ProtoBackendTensor::set_type(const std::string& type) {
  ONNX_NAMESPACE::TensorProto::DataType dt;
  if (!ONNX_NAMESPACE::TensorProto::DataType_Parse(type, &dt)) {
    return false;
  }
  handle_->set_data_type(dt);
  return true;
}
std::vector<int64_t> ProtoBackendTensor::dim() const {
  // TODO(oPluss): impl
  return {};
}
bool ProtoBackendTensor::set_dim(const std::vector<int64_t>& dim) {
  // TODO(oPluss): impl
  return false;
}
void* ProtoBackendTensor::data() const {
  // TODO(oPluss): impl
  return nullptr;
}
size_t ProtoBackendTensor::data_size() const {
  // TODO(oPluss): impl
  return 0;
}
bool ProtoBackendTensor::set_data(void* ptr, size_t len) {
  // TODO(oPluss): impl
  return false;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
