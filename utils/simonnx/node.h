/**
 * @file node.h
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

#ifndef UTILS_SIMONNX_NODE_H_
#define UTILS_SIMONNX_NODE_H_

#include <any>
#include <functional>
#include <list>
#include <string>
#include <vector>

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/object.h"
#include "utils/simonnx/treaty.h"

namespace utils {
namespace simonnx {

using NodeObjBase = Object<ObjType_t::kNode>;
using SBackendNode = backend::SBackendNode;
using SBackendValueInfo = backend::SBackendValueInfo;
using SBackendTensor = backend::SBackendTensor;

class SimOnnxCtx;

struct FakeNode_t {
  std::string fake_op_type;
  std::string fake_name;
  std::vector<std::string> fake_inputs;
  std::vector<std::string> fake_outputs;
};

class NodeObj : public NodeObjBase {
 public:
  static NodeObj* Create(SimOnnxCtx* ctx, FakeNode_t args);
  static NodeObj* Create(SimOnnxCtx* ctx, SBackendNode handle);
  enum IONodeType { kInputNode = 0, kOutputNode = 1 };
  static NodeObj* Create(SimOnnxCtx* ctx, SBackendValueInfo handle,
                         IONodeType type);
  static NodeObj* Create(SimOnnxCtx* ctx, SBackendTensor handle);

 public:
  explicit NodeObj(SimOnnxCtx* ctx) : NodeObjBase(ctx) {}
  ~NodeObj() = default;

  virtual std::string getName() = 0;
  virtual bool setName(std::string name) { return false; }
  virtual std::string getOpType() = 0;
  virtual bool setOpType(std::string op_type) { return false; }
  virtual std::vector<std::string> getInputs() = 0;
  virtual bool setInputs(const std::vector<std::string>& inputs) {
    return false;
  }
  virtual std::vector<std::string> getOutputs() = 0;
  virtual bool setOutputs(const std::vector<std::string>& outputs) {
    return false;
  }
};

using NodeHandle = utils::simonnx::NodeObj*;

class FakeNodeObj : public NodeObj {
 public:
  explicit FakeNodeObj(SimOnnxCtx* ctx, FakeNode_t args)
      : NodeObj(ctx), faked_(args) {
    setAttr("setName", "false");
    setAttr("setOpType", "false");
    setAttr("setInputs", "false");
    setAttr("setOutputs", "false");
    setAttr("setDim", "false");
    setAttr("NodeType", "FakeNode");
  }
  std::string getName() override { return faked_.fake_name; }
  std::string getOpType() override { return faked_.fake_op_type; }
  std::vector<std::string> getInputs() override { return faked_.fake_inputs; }
  std::vector<std::string> getOutputs() override { return faked_.fake_outputs; }
  bool destroyHandle() override { return true; }

 private:
  FakeNode_t faked_;
};

class RealNodeObj : public NodeObj {
 public:
  explicit RealNodeObj(SimOnnxCtx* ctx, SBackendNode handle)
      : NodeObj(ctx), handle_(handle) {
    setAttr("setName", "true");
    setAttr("setOpType", "true");
    setAttr("setInputs", "true");
    setAttr("setOutputs", "true");
    setAttr("setDim", "false");
    setAttr("NodeType", "RealNode");
  }
  std::string getName() override;
  bool setName(std::string name) override;
  std::string getOpType() override;
  bool setOpType(std::string op_type) override;
  std::vector<std::string> getInputs() override;
  bool setInputs(const std::vector<std::string>& inputs) override;
  std::vector<std::string> getOutputs() override;
  bool setOutputs(const std::vector<std::string>& outputs) override;
  bool destroyHandle() override;

 private:
  SBackendNode handle_;
};

class IONodeObj : public NodeObj {
 public:
  explicit IONodeObj(SimOnnxCtx* ctx, SBackendValueInfo handle)
      : NodeObj(ctx), handle_(handle) {
    setAttr("setName", "true");
    setAttr("setOpType", "false");
    setAttr("setInputs", "false");
    setAttr("setOutputs", "false");
    setAttr("setDim", "true");
    setAttr("NodeType", "IONode");
  }
  std::string getName() override;
  bool setName(std::string name) override;
  std::vector<int64_t> getDim();
  bool setDim(const std::vector<int64_t>& dims);
  std::string getDataType();
  bool setDataType(const std::string& datatype);

 protected:
  SBackendValueInfo handle_;
};

class InputNodeObj : public IONodeObj {
 public:
  explicit InputNodeObj(SimOnnxCtx* ctx, SBackendValueInfo handle)
      : IONodeObj(ctx, handle) {}
  std::string getOpType() override { return TREATY_INPUT_OP_TYPE; }
  std::vector<std::string> getInputs() override { return {}; }
  std::vector<std::string> getOutputs() override;
  // bool setOutputs(const std::vector<std::string>& outputs) override;
  bool destroyHandle() override;
};

class OutputNodeObj : public IONodeObj {
 public:
  explicit OutputNodeObj(SimOnnxCtx* ctx, SBackendValueInfo handle)
      : IONodeObj(ctx, handle) {}
  std::string getOpType() override { return TREATY_OUTPUT_OP_TYPE; }
  std::vector<std::string> getInputs() override;
  // bool setInputs(const std::vector<std::string>& inputs) override;
  std::vector<std::string> getOutputs() override { return {}; }
  bool destroyHandle() override;
};

class InitNodeObj : public NodeObj {
 public:
  explicit InitNodeObj(SimOnnxCtx* ctx, SBackendTensor handle)
      : NodeObj(ctx), handle_(handle) {
    setAttr("setName", "true");
    setAttr("setOpType", "false");
    setAttr("setInputs", "false");
    setAttr("setOutputs", "false");
    setAttr("setDim", "false");
    setAttr("NodeType", "InitNode");
  }
  std::string getName() override;
  bool setName(std::string name) override;
  std::string getOpType() override { return TREATY_INIT_OP_TYPE; }
  std::vector<std::string> getInputs() override { return {}; }
  std::vector<std::string> getOutputs() override { return {getName()}; }
  // bool setOutputs(const std::vector<std::string>& outputs) override;
  bool destroyHandle() override;

 private:
  SBackendTensor handle_;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_NODE_H_
