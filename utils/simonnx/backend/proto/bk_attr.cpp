/**
 * @file bk_attr.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-11 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/backend/proto/bk_attr.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_node.h"
#include "utils/simonnx/backend/proto/helper.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

using Data_t = ProtoBackendAttribute::Data_t;

ProtoBackendAttribute::ProtoBackendAttribute(ProtoBackendNode* parent,
                                             AttributeProtoPtr handle)
    : parent_(parent), handle_(handle) {}

bool ProtoBackendAttribute::destroy() {
  VLOG(1) << "delete NodeProtoPtr";
  auto attrs = parent_->getHandle()->mutable_attribute();
  if (delFromRepeatProto(attrs, handle_)) {
    handle_ = nullptr;
    return true;
  } else {
    LOG(ERROR) << "delete NodeProtoPtr failed @" << handle_;
    return false;
  }
}

std::string ProtoBackendAttribute::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}

bool ProtoBackendAttribute::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}
Data_t ProtoBackendAttribute::getData() const {
  CHECK_HANDLE_DEL({});
  // TODO(oPluss): Impl
  return {};
}
bool ProtoBackendAttribute::setData(Data_t data) {
  CHECK_HANDLE_DEL(false);
  // TODO(oPluss): Impl
  return false;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
