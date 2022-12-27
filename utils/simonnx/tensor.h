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

namespace ONNX_NAMESPACE {
class TensorProto;
class ValueInfoProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {

using TensorObjBase = Object<ObjType_t::kTensor>;
using TensorProtoPtr = ::ONNX_NAMESPACE::TensorProto*;
using ValueInfoProtoPtr = ::ONNX_NAMESPACE::ValueInfoProto*;

struct FakeTensor_t {
  std::string fake_name;
};

class TensorObj : public TensorObjBase {
 public:
  static TensorObj* Create(SimOnnxCtx* ctx, FakeTensor_t args);
  // static TensorObj* Create(SimOnnxCtx* ctx, TensorProtoPtr handle);
  static TensorObj* Create(SimOnnxCtx* ctx, ValueInfoProtoPtr handle);

 public:
  explicit TensorObj(SimOnnxCtx* ctx) : TensorObjBase(ctx) {}
  virtual ~TensorObj() {}
  virtual std::string getName() = 0;
  virtual bool setName(std::string name) { return false; }
};

using TensorHandle = utils::simonnx::TensorObj*;

class FakeTensorObj : public TensorObj {
 public:
  explicit FakeTensorObj(SimOnnxCtx* ctx, FakeTensor_t args)
      : TensorObj(ctx), faked_(args) {
    setAttr("setName", "false");
  }
  std::string getName() override { return faked_.fake_name; }

 protected:
  bool destroyHandle() override { return true; }

 private:
  FakeTensor_t faked_;
};

class InitTensorObj : public TensorObj {
 public:
  explicit InitTensorObj(SimOnnxCtx* ctx, TensorProtoPtr handle)
      : TensorObj(ctx), handle_(handle) {
    setAttr("setName", "false");
  }
  std::string getName() override;
  // bool setName(std::string name) override;

 protected:
  bool destroyHandle() override;

 private:
  TensorProtoPtr handle_;
};

class ValueTensorObj : public TensorObj {
 public:
  explicit ValueTensorObj(SimOnnxCtx* ctx, ValueInfoProtoPtr handle)
      : TensorObj(ctx), handle_(handle) {
    setAttr("setName", "false");
  }
  std::string getName() override;
  // bool setName(std::string name) override;

 protected:
  bool destroyHandle() override;

 private:
  ValueInfoProtoPtr handle_;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_TENSOR_H_
