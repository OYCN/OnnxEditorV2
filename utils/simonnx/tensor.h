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

#include "utils/algorithm/parse.h"
#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/object.h"

namespace utils {
namespace simonnx {

using DimStr = utils::algorithm::parse::DimStr;

using TensorObjBase = Object<ObjType_t::kTensor>;
using SBackendTensor = backend::SBackendTensor;
using SBackendValueInfo = backend::SBackendValueInfo;

struct FakeTensor_t {
  std::string fake_name;
};

class TensorObj : public TensorObjBase {
 public:
  static TensorObj* Create(SimOnnxCtx* ctx, FakeTensor_t args);
  // static TensorObj* Create(SimOnnxCtx* ctx, SBackendTensor handle);
  static TensorObj* Create(SimOnnxCtx* ctx, SBackendValueInfo handle);

 public:
  explicit TensorObj(SimOnnxCtx* ctx) : TensorObjBase(ctx) {}
  virtual ~TensorObj() {}
  virtual std::string getName() const = 0;
  virtual bool setName(std::string name) { return false; }
  virtual DimStr getDim() const = 0;
  virtual bool setShape(std::vector<int64_t> shape) { return false; }
  virtual std::string getDataType() const = 0;
  virtual bool setDataType(const std::string& datatype) { return false; }
};

using TensorHandle = utils::simonnx::TensorObj*;

class FakeTensorObj : public TensorObj {
 public:
  explicit FakeTensorObj(SimOnnxCtx* ctx, FakeTensor_t args)
      : TensorObj(ctx), faked_(args) {
    setAttr("setName", "false");
  }
  std::string getName() const override { return faked_.fake_name; }
  DimStr getDim() const override { return DimStr({}); }
  std::string getDataType() const override { return ""; }
  bool destroyHandle() override { return true; }

 private:
  FakeTensor_t faked_;
};

class InitTensorObj : public TensorObj {
 public:
  explicit InitTensorObj(SimOnnxCtx* ctx, SBackendTensor handle)
      : TensorObj(ctx), handle_(handle) {
    setAttr("setName", "false");
  }
  std::string getName() const override;
  // bool setName(std::string name) override;
  DimStr getDim() const override;
  std::string getDataType() const override;
  bool destroyHandle() override;

 private:
  SBackendTensor handle_;
};

class ValueTensorObj : public TensorObj {
 public:
  explicit ValueTensorObj(SimOnnxCtx* ctx, SBackendValueInfo handle)
      : TensorObj(ctx), handle_(handle) {
    setAttr("setName", "false");
  }
  std::string getName() const override;
  // bool setName(std::string name) override;
  DimStr getDim() const override;
  std::string getDataType() const override;
  bool destroyHandle() override;

 private:
  SBackendValueInfo handle_;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_TENSOR_H_
