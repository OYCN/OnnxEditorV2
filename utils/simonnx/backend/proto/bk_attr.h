/**
 * @file bk_attr.h
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

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_ATTR_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_ATTR_H_

#include "utils/simonnx/backend/backend.h"

namespace ONNX_NAMESPACE {
class AttributeProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

class ProtoBackendNode;

using AttributeProtoPtr = ::ONNX_NAMESPACE::AttributeProto*;

class ProtoBackendAttribute
    : public ::utils::simonnx::backend::IBackendAttribute {
 public:
  using Data_t = IBackendAttribute::Data_t;

 public:
  ProtoBackendAttribute(ProtoBackendNode* parent, AttributeProtoPtr handle);
  bool destroy() override;

  std::string name() const override;
  bool set_name(const std::string& name) override;
  Data_t getData() const override;
  bool setData(Data_t data) override;

 public:
  AttributeProtoPtr getHandle() { return handle_; }

 private:
  ProtoBackendNode* parent_;
  AttributeProtoPtr handle_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_ATTR_H_
