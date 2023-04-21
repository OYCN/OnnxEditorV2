/**
 * @file bk_value_info.h
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

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_VALUE_INFO_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_VALUE_INFO_H_

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/backend/proto/bk_ctx.h"

namespace ONNX_NAMESPACE {
class ValueInfoProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

class ProtoBackendGraph;

using ValueInfoProtoPtr = ::ONNX_NAMESPACE::ValueInfoProto*;

class ProtoBackendValueInfo
    : public ::utils::simonnx::backend::IBackendValueInfo {
 public:
  ProtoBackendValueInfo(ProtoBackendGraph* parent, ValueInfoProtoPtr handle);
  bool destroy() override;

  std::string name() const override;
  bool set_name(const std::string& name) override;
  std::string type() const override;
  bool set_type(const std::string& type) override;
  std::vector<DimVal> dim() const override;
  bool set_dim(const std::vector<DimVal>& dim) override;

 public:
  ValueInfoProtoPtr getHandle() { return handle_; }

 private:
  ProtoBackendGraph* parent_;
  ValueInfoProtoPtr handle_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_VALUE_INFO_H_
