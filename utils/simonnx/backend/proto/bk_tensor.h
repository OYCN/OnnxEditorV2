/**
 * @file bk_tensor.h
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

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_TENSOR_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_TENSOR_H_

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/backend/proto/bk_ctx.h"

namespace ONNX_NAMESPACE {
class TensorProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

class ProtoBackendGraph;

using TensorProtoPtr = ::ONNX_NAMESPACE::TensorProto*;

class ProtoBackendTensor : public ::utils::simonnx::backend::IBackendTensor {
 public:
  ProtoBackendTensor(ProtoBackendGraph* parent, TensorProtoPtr handle);
  bool destroy() override;

  std::string name() const override;
  bool set_name(const std::string& name) override;
  std::string type() const override;
  bool set_type(const std::string& type) override;
  std::vector<int64_t> dim() const override;
  bool set_dim(const std::vector<int64_t>& dim) override;
  void* data() const override;
  size_t data_size() const override;
  bool set_data(void* ptr, size_t len) override;

 public:
  TensorProtoPtr getHandle() { return handle_; }

 private:
  ProtoBackendGraph* parent_;
  TensorProtoPtr handle_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_TENSOR_H_
