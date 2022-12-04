/**
 * @file node.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-04 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/node.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

namespace utils {
namespace simonnx {

using NodeProto = ::ONNX_NAMESPACE::NodeProto;

NodeObj::NodeObj(const ::ONNX_NAMESPACE::NodeProto* handle) {
  CHECK(handle != nullptr);
  handle_ = new NodeProto(*handle);
}

NodeObj::~NodeObj() { delete handle_; }

std::string NodeObj::getName() { return handle_->name(); }

void NodeObj::setName(std::string name) { handle_->set_name(name); }

std::string NodeObj::getOpType() { return handle_->op_type(); }

void NodeObj::setOpType(std::string op_type) { handle_->set_op_type(op_type); }

}  // namespace simonnx
}  // namespace utils
