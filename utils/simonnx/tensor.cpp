/**
 * @file tensor.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-05 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/tensor.h"

#include <onnx/onnx_pb.h>

#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

TensorObj* TensorObj::Create(SimOnnxCtx* ctx, FakeTensor_t args) {
  return new FakeTensorObj(ctx, args);
}
// TensorObj* TensorObj::Create(SimOnnxCtx* ctx, TensorProtoPtr handle) {
//   return new InitTensorObj(ctx, handle);
// }
TensorObj* TensorObj::Create(SimOnnxCtx* ctx, ValueInfoProtoPtr handle) {
  return new ValueTensorObj(ctx, handle);
}

std::string InitTensorObj::getName() { return handle_->name(); }
// bool InitTensorObj::setName(std::string name) {
//   handle_->set_name(name);
//   return true;
// }

bool InitTensorObj::destroyHandle() { return getCtx()->destroyHandle(handle_); }

std::string ValueTensorObj::getName() { return handle_->name(); }
// bool ValueTensorObj::setName(std::string name) {
//   handle_->set_name(name);
//   return true;
// }

bool ValueTensorObj::destroyHandle() {
  return getCtx()->destroyHandle(handle_);
}

}  // namespace simonnx
}  // namespace utils
