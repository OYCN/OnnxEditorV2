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

class NodeObj : public NodeObjBase {
 public:
  explicit NodeObj(SimOnnxCtx* ctx, ::ONNX_NAMESPACE::NodeProto* handle,
                   bool own = false);
  ~NodeObj();

  std::string getName();
  void setName(std::string name);
  std::string getOpType();
  void setOpType(std::string op_type);

 private:
  NodeProtoPtr handle_;
  bool own_;
};

using NodeHandle = utils::simonnx::NodeObj*;

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_NODE_H_
