/**
 * @file onnx_pass.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-26 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_BACKEND_PROTO_ONNX_PASS_H_
#define UTILS_SIMONNX_BACKEND_PROTO_ONNX_PASS_H_

namespace ONNX_NAMESPACE {
class ModelProto;
};  // namespace ONNX_NAMESPACE

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

using ModelProtoPtr = ::ONNX_NAMESPACE::ModelProto*;

class OnnxPass {
 public:
  static bool pass_all(ModelProtoPtr mp) {
    bool ret = true;
    ret = ret && node_topo_sort(mp);
    return ret;
  }

  static bool remove_useless_node(ModelProtoPtr mp);
  static bool remove_useless_tensor(ModelProtoPtr mp);
  static bool node_topo_sort(ModelProtoPtr mp);
};

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_ONNX_PASS_H_
