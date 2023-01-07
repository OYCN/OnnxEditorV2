/**
 * @file helper.h
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

#ifndef UTILS_SIMONNX_BACKEND_PROTO_HELPER_H_
#define UTILS_SIMONNX_BACKEND_PROTO_HELPER_H_

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

template <typename RPT, typename VT>
bool delFromRepeatProto(RPT rp, VT handle) {
  for (auto iter = rp->begin(); iter != rp->end();) {
    if (&(*iter) == handle) {
      iter = rp->erase(iter);
      VLOG(1) << "delete success";
      return true;
    } else {
      iter++;
    }
  }
  return false;
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_PROTO_HELPER_H_
