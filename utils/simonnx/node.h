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

#include "utils/simonnx/object.h"

namespace ONNX_NAMESPACE {
class NodeProto;
};

namespace utils {
namespace simonnx {

using NodeObjBase = Object<ObjType_t::kNode>;
using NodeProtoPtr = ::ONNX_NAMESPACE::NodeProto*;

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
  static NodeObj* Create(SimOnnxCtx* ctx, NodeProtoPtr handle);

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
    setAttr("writable", "false");
  }
  std::string getName() override { return faked_.fake_name; }
  std::string getOpType() override { return faked_.fake_op_type; }
  std::vector<std::string> getInputs() override { return faked_.fake_inputs; }
  std::vector<std::string> getOutputs() override { return faked_.fake_outputs; }

 private:
  FakeNode_t faked_;
};

class RealNodeObj : public NodeObj {
 public:
  explicit RealNodeObj(SimOnnxCtx* ctx, NodeProtoPtr handle)
      : NodeObj(ctx), handle_(handle) {
    setAttr("writable", "true");
  }
  std::string getName() override;
  bool setName(std::string name) override;
  std::string getOpType() override;
  bool setOpType(std::string op_type) override;
  std::vector<std::string> getInputs() override;
  bool setInputs(const std::vector<std::string>& inputs) override;
  std::vector<std::string> getOutputs() override;
  bool setOutputs(const std::vector<std::string>& outputs) override;

 private:
  NodeProtoPtr handle_;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_NODE_H_
