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

NodeObj* NodeObj::Create(SimOnnxCtx* ctx, FakeNode_t args) {
  return new FakeNodeObj(ctx, args);
}
NodeObj* NodeObj::Create(SimOnnxCtx* ctx, NodeProtoPtr handle) {
  return new RealNodeObj(ctx, handle);
}

std::string RealNodeObj::getName() { return handle_->name(); }

bool RealNodeObj::setName(std::string name) {
  handle_->set_name(name);
  return true;
}

std::string RealNodeObj::getOpType() { return handle_->op_type(); }

bool RealNodeObj::setOpType(std::string op_type) {
  handle_->set_op_type(op_type);
  return true;
}

}  // namespace simonnx
}  // namespace utils
