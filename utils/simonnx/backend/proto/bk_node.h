/**
 * @file bk_node.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-07 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_BACKEND_PROTO_BK_NODE_H_
#define UTILS_SIMONNX_BACKEND_PROTO_BK_NODE_H_

#include "utils/simonnx/backend/backend.h"
#include "utils/simonnx/backend/proto/bk_ctx.h"

namespace ONNX_NAMESPACE {
class NodeProto;
class AttributeProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

class ProtoBackendGraph;

using NodeProtoPtr = ::ONNX_NAMESPACE::NodeProto*;
using AttributeProtoPtr = ::ONNX_NAMESPACE::AttributeProto*;

class ProtoBackendNode : public ::utils::simonnx::backend::IBackendNode {
 public:
  ProtoBackendNode(ProtoBackendGraph* parent, NodeProtoPtr handle);
  bool destroy() override;

  std::string name() const override;
  bool set_name(const std::string& name) override;
  std::string op_type() const override;
  bool set_op_type(const std::string& op_type) override;
  std::vector<std::string> input() const override;
  bool set_input(const std::vector<std::string>& input) override;
  std::vector<std::string> output() const override;
  bool set_output(const std::vector<std::string>& output) override;
  const std::vector<SBackendAttribute>& attribute() const override;
  SBackendAttribute add_attribute() override;
  bool del_attribute(SBackendAttribute attr) override;

 public:
  NodeProtoPtr getHandle() { return handle_; }

 private:
  ProtoBackendGraph* parent_;
  NodeProtoPtr handle_;

  std::vector<SBackendAttribute> attr_;
};

class ProtoBackendAttribute
    : public ::utils::simonnx::backend::IBackendAttribute {
 public:
  ProtoBackendAttribute(ProtoBackendNode* parent, AttributeProtoPtr handle);

  std::string name() const override;
  bool set_name(const std::string& name) override;

 public:
  AttributeProtoPtr getHandle() { return handle_; }

 private:
  ProtoBackendNode* parent_;
  AttributeProtoPtr handle_;
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_BK_NODE_H_
