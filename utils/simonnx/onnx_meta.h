/**
 * @file meta.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-19 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_ONNX_META_H_
#define UTILS_SIMONNX_ONNX_META_H_

#include <onnx/onnx_pb.h>

#include <map>
#include <string>

namespace utils {
namespace simonnx {

struct OnnxMeta {
  using Shape_t = std::vector<int64_t>;
  std::vector<std::string> inputs;
  std::vector<std::string> outputs;
  std::map<std::string, size_t> name_to_node_idx_;
  std::map<std::string, size_t> name_to_initializer_idx_;
  std::map<std::string, std::string> name_as_node_input_;
  std::map<std::string, std::string> name_as_node_output_;
  std::map<std::string, Shape_t> name_to_shape_;
};

OnnxMeta genOnnxMeta(const ::ONNX_NAMESPACE::ModelProto& model);

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_ONNX_META_H_
