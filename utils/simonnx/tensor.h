/**
 * @file tensor.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-03 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_TENSOR_H_
#define UTILS_SIMONNX_TENSOR_H_

#include <functional>
#include <list>
#include <string>
#include <vector>

#include "utils/simonnx/object.h"

namespace utils {
namespace simonnx {

using TensorObjBase = Object<ObjType_t::kTensor>;

class TensorObj : public TensorObjBase {
 public:
  explicit TensorObj(SimOnnxCtx* ctx, std::string name = "")
      : TensorObjBase(ctx), name_(name) {}
  ~TensorObj() {}
  std::string getName() { return name_; }
  void setName(std::string name) { name_ = name; }

 private:
  std::string name_;
};

using TensorHandle = utils::simonnx::TensorObj*;

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_TENSOR_H_
