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

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

TensorObj* TensorObj::Create(SimOnnxCtx* ctx, FakeTensor_t args) {
  return new FakeTensorObj(ctx, args);
}
// TensorObj* TensorObj::Create(SimOnnxCtx* ctx, SBackendTensor handle) {
//   return new InitTensorObj(ctx, handle);
// }
TensorObj* TensorObj::Create(SimOnnxCtx* ctx, SBackendValueInfo handle) {
  return new ValueTensorObj(ctx, handle);
}

std::string InitTensorObj::getName() const { return handle_->name(); }
// bool InitTensorObj::setName(std::string name) {
//   handle_->set_name(name);
//   return true;
// }
DimStr InitTensorObj::getDim() const { return DimStr(handle_->dim()); }
std::string InitTensorObj::getDataType() const { return handle_->type(); }

bool InitTensorObj::destroyHandle() { return handle_->destroy(); }

std::string ValueTensorObj::getName() const { return handle_->name(); }
// bool ValueTensorObj::setName(std::string name) {
//   handle_->set_name(name);
//   return true;
// }
DimStr ValueTensorObj::getDim() const { return DimStr(handle_->dim()); }
std::string ValueTensorObj::getDataType() const { return handle_->type(); }

bool ValueTensorObj::destroyHandle() { return handle_->destroy(); }

}  // namespace simonnx
}  // namespace utils
