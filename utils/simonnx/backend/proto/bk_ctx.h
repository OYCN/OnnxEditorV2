/**
 * @file bk_ctx.h
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

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_CTX_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_CTX_H_

#include "utils/simonnx/backend/backend.h"

namespace ONNX_NAMESPACE {
class ModelProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

using ModelProtoPtr = ::ONNX_NAMESPACE::ModelProto*;

class ProtoBackendCtx : public ::utils::simonnx::backend::IBackendCtx {
 public:
  ProtoBackendCtx();
  ~ProtoBackendCtx();
  bool loadFile(const std::string& path) override;
  bool saveFile(const std::string& path, bool overwrite = true) override;
  SBackendGraph graph() const override;
  bool pass(PassType type) override;

 public:
  ModelProtoPtr getHandle() { return mp_; }

 private:
  ModelProtoPtr mp_;
  SBackendGraph graph_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_CTX_H_
