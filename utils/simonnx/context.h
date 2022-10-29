/**
 * @file context.h
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

#ifndef UTILS_SIMONNX_CONTEXT_H_
#define UTILS_SIMONNX_CONTEXT_H_

#include <list>
#include <memory>
#include <type_traits>

#include "utils/simonnx/node.h"
#include "utils/simonnx/tensor.h"

namespace utils {
namespace simonnx {

class SimOnnxCtx {
 public:
  SimOnnxCtx() = delete;

  template <typename... _Args>
  static NodeHandle CreateNodeObj(_Args&&... args) {
    return CreateObj<NodeHandle, _Args...>(std::forward<_Args>(args)...);
  }
  template <typename... _Args>
  static TensorHandle CreateTensorObj(_Args&&... args) {
    return CreateObj<TensorHandle, _Args...>(std::forward<_Args>(args)...);
  }

 private:
  template <typename _Type>
  static void savePointer(_Type* ptr) {
    static std::list<std::unique_ptr<_Type>> list;
    list.emplace_back(ptr);
  }
  template <typename _Ret, typename... _Args>
  static _Ret CreateObj(_Args&&... args) {
    using ObjType = typename std::remove_pointer<_Ret>::type;
    auto ptr = new ObjType(std::forward<_Args>(args)...);
    savePointer(ptr);
    return ptr;
  }
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_CONTEXT_H_
