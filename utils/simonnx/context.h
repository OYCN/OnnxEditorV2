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
#include <map>
#include <memory>
#include <type_traits>

#include "utils/algorithm/graph_desc.h"
#include "utils/simonnx/node.h"
#include "utils/simonnx/tensor.h"

namespace ONNX_NAMESPACE {
class ModelProto;
class GraphProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {

using ModelProtoPtr = ::ONNX_NAMESPACE::ModelProto*;
using GraphProtoPtr = ::ONNX_NAMESPACE::GraphProto*;

class SimOnnxCtx {
 public:
  static SimOnnxCtx* createSimOnnxCtx() {
    static size_t idx = 0;
    SimOnnxCtx new_ctx;
    getCtxMap().emplace(idx, std::move(new_ctx));
    auto ptr = &getCtxMap().at(idx++);
    return ptr;
  }
  static SimOnnxCtx* getSimOnnxCtx(size_t idx = 0) {
    return &getCtxMap().at(idx);
  }

 public:
  template <typename... _Args>
  NodeHandle CreateNodeObj(_Args&&... args) {
    return CreateObj<NodeHandle, _Args...>(std::forward<_Args>(args)...);
  }
  template <typename... _Args>
  TensorHandle CreateTensorObj(_Args&&... args) {
    return CreateObj<TensorHandle, _Args...>(std::forward<_Args>(args)...);
  }

  ModelProtoPtr getModelProtoPtr() { return mp_; }
  bool openOnnx(const std::string path);
  void reset();

 private:
  SimOnnxCtx();

  template <typename _Ret, typename... _Args>
  _Ret CreateObj(_Args&&... args) {
    using ObjType = typename std::remove_pointer<_Ret>::type;
    auto ptr = ObjType::Create(this, std::forward<_Args>(args)...);
    ptr->setId(obj_ctx_[ObjType::ObjType].size());
    obj_ctx_[ObjType::ObjType].emplace_back(ptr);
    return ptr;
  }

  static std::map<size_t, SimOnnxCtx>& getCtxMap() {
    static std::map<size_t, SimOnnxCtx> ctxes;
    return ctxes;
  }

 private:
  std::map<ObjType_t, std::list<IObject*>> obj_ctx_;
  ModelProtoPtr mp_;
};

using SimOnnxCtxHandle = SimOnnxCtx*;

using GraphNode2NodeDescExtTmp =
    utils::algorithm::desc::GraphNode2NodeDescExtTmp<NodeHandle, TensorHandle,
                                                     SimOnnxCtxHandle>;
using GraphNode2NodeDescExt =
    utils::algorithm::desc::GraphNode2NodeDescExt<NodeHandle, TensorHandle,
                                                  SimOnnxCtxHandle>;

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_CONTEXT_H_
