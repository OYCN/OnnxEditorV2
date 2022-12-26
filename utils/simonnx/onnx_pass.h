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

#ifndef UTILS_SIMONNX_ONNX_PASS_H_
#define UTILS_SIMONNX_ONNX_PASS_H_

namespace utils {
namespace simonnx {

class SimOnnxCtx;

class OnnxPass {
 public:
  static bool pass_all(SimOnnxCtx* ctx) {
    bool ret = true;
    ret = ret && remove_deleted_node(ctx);
    ret = ret && node_topo_sort(ctx);
    return ret;
  }

  static bool remove_useless_tensor(SimOnnxCtx* ctx);
  static bool remove_useless_node(SimOnnxCtx* ctx);
  static bool remove_deleted_node(SimOnnxCtx* ctx);
  static bool node_topo_sort(SimOnnxCtx* ctx);
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_ONNX_PASS_H_
