/**
 * @file context.cpp
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

#include "utils/simonnx/context.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include <cstdio>

namespace utils {
namespace simonnx {

SimOnnxCtx::SimOnnxCtx() { mp_ = new ::ONNX_NAMESPACE::ModelProto(); }

}  // namespace simonnx
}  // namespace utils
