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
#include <mutex>  // NOLINT [build/c++11]
#include <type_traits>

#include "utils/algorithm/graph_desc.h"
#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/node.h"
#include "utils/simonnx/tensor.h"
#include "utils/simonnx/treaty.h"

namespace utils {
namespace simonnx {

using BackendType = backend::BackendType;
using PassType = backend::PassType;

class SimOnnxCtx {
 public:
  explicit SimOnnxCtx(BackendType type);
  ~SimOnnxCtx();

  static SimOnnxCtx* createSimOnnxCtx(BackendType type = BackendType::kProto) {
    static std::mutex mutex;
    std::unique_lock lock(mutex);
    static size_t idx = 0;
    getCtxMap().emplace(idx, type);
    auto ptr = getSimOnnxCtx(idx++);
    return ptr;
  }
  static SimOnnxCtx* getSimOnnxCtx(size_t idx = 0) {
    return &getCtxMap().at(idx);
  }

 public:
  template <typename... _Args>
  NodeHandle CreateNodeObj(_Args&&... args) {
    std::unique_lock lock(mutex_);
    return CreateNodeObjImpl<_Args...>(std::forward<_Args>(args)...);
  }
  template <typename... _Args>
  TensorHandle CreateTensorObj(_Args&&... args) {
    std::unique_lock lock(mutex_);
    return CreateTensorObjImpl<_Args...>(std::forward<_Args>(args)...);
  }
  NodeHandle CreateNewNodeObj();
  void DeleteObj(IObject* obj);
  void RestoreObj(IObject* obj);
  template <typename _T>
  const std::vector<_T> getObjVec() {
    std::vector<_T> ret;
    using ObjType = typename std::remove_pointer<_T>::type;
    if (obj_free_ctx_.count(ObjType::ObjType), 1) {
      if (obj_free_ctx_.count(ObjType::ObjType) != 0) {
        for (const auto& o : obj_free_ctx_.at(ObjType::ObjType)) {
          ret.emplace_back(dynamic_cast<_T>(o));
        }
      }
    }
    return ret;
  }

  void genRandomOnnx(int num);
  bool openOnnx(const std::string path);
  bool saveOnnx(const std::string path, bool overwrite);
  void reset();
  bool applyDeletedObj();
  bool ctxPass(PassType type);
  void setDebugFn(std::function<void(std::string)> fn);
  void setInfoFn(std::function<void(std::string)> fn);
  void setErrorFn(std::function<void(std::string)> fn);
  void resetDebugFn();
  void resetInfoFn();
  void resetErrorFn();

 private:
  template <typename _Ret, typename... _Args>
  _Ret CreateObjImpl(_Args&&... args) {
    using ObjType = typename std::remove_pointer<_Ret>::type;
    auto ptr = ObjType::Create(this, std::forward<_Args>(args)...);
    ptr->setId(obj_free_ctx_[ObjType::ObjType].size());
    obj_free_ctx_[ObjType::ObjType].emplace_back(ptr);
    ptr->setIter(std::prev(obj_free_ctx_[ObjType::ObjType].end()));
    ptr->setDeleted(false);
    return ptr;
  }
  template <typename... _Args>
  NodeHandle CreateNodeObjImpl(_Args&&... args) {
    return CreateObjImpl<NodeHandle, _Args...>(std::forward<_Args>(args)...);
  }
  template <typename... _Args>
  TensorHandle CreateTensorObjImpl(_Args&&... args) {
    return CreateObjImpl<TensorHandle, _Args...>(std::forward<_Args>(args)...);
  }
  static std::map<size_t, SimOnnxCtx>& getCtxMap() {
    static std::map<size_t, SimOnnxCtx> ctxes;
    return ctxes;
  }
  void reset_impl();

 private:
  std::mutex mutex_;
  std::map<ObjType_t, std::list<IObject*>> obj_free_ctx_;
  std::map<ObjType_t, std::list<IObject*>> obj_del_ctx_;
  std::shared_ptr<backend::IBackendCtx> bkctx_;
  std::function<void(std::string)> infofn_;
  std::function<void(std::string)> errorfn_;
  std::function<void(std::string)> debugfn_;
};

using SimOnnxCtxHandle = SimOnnxCtx*;

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_CONTEXT_H_
