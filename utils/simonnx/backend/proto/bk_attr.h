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
  ProtoBackendAttribute(ProtoBackendNode* parent, AttributeProtoPtr handle);
  bool destroy() override;

  std::string name() const override;
  bool set_name(const std::string& name) override;
  std::string type() const override;
  int i() const override;
  bool set_i(int v) override;
  float f() const override;
  bool set_f(float v) override;
  std::string s() const override;
  bool set_s(std::string s) override;
  std::vector<int> ints() const override;
  bool set_ints(std::vector<int> vs) override;
  std::vector<float> floats() const override;
  bool set_floats(std::vector<float> vs) override;

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
