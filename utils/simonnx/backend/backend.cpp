/**
 * @file backend.cpp
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
#include "utils/simonnx/backend/backend.h"

#include <glog/logging.h>

#include "utils/simonnx/backend/proto/bk_ctx.h"

namespace utils {
namespace simonnx {
namespace backend {

std::shared_ptr<IBackendCtx> IBackendCtx::createCtx(BackendType type) {
  std::shared_ptr<IBackendCtx> ptr;
  switch (type) {
    case BackendType::kProto:
      ptr = std::make_shared<proto::ProtoBackendCtx>();
      break;
    case BackendType::kOnnxIR:
      LOG(FATAL) << "unimpl";
      break;
    default:
      LOG(FATAL) << "err";
      break;
  }
  if (ptr) {
    ptr->setSptr(ptr);
  }
  return ptr;
}

}  // namespace backend
}  // namespace simonnx
}  // namespace utils
