/**
 * @file parser.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/onnx/parser.h"

#include <google/protobuf/text_format.h>
#include <onnx/onnx_pb.h>

namespace utils {
namespace onnx {

Parser::Parser() { GOOGLE_PROTOBUF_VERIFY_VERSION; }

SimOnnx Parser::parse(void* data, std::size_t size) {
  ::ONNX_NAMESPACE::ModelProto onnx_model;
  bool ret = onnx_model.ParseFromArray(data, size);
  if (ret) {
    // TODO(opluss) parse onnx
  } else {
    return SimOnnx{.is_valid = false};
  }
}

}  // namespace onnx
}  // namespace utils
