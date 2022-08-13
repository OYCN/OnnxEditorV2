/**
 * @file onnx_meta.cpp
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

#include "utils/simonnx/onnx_meta.h"

#include <glog/logging.h>

#define CHECK_RETURN(cond, def) \
  do {                          \
    bool ret = (cond);          \
    if (!ret) {                 \
      return def;               \
    }                           \
  } while (0)

namespace utils {
namespace simonnx {

template <typename _K, typename _V>
void empty_and_insert(std::map<_K, _V>& map, const _K& key, const _V& val) {
  CHECK_EQ(map.count(key), 0) << "Name of " << key << " has been inserted";
  map[key] = val;
}

OnnxMeta::Shape_t handleValueInfoProtoToShape(
    const ::ONNX_NAMESPACE::ValueInfoProto& vi) {
  CHECK_RETURN(vi.has_type() && vi.type().has_tensor_type() &&
                   vi.type().tensor_type().has_shape(),
               {});
  OnnxMeta::Shape_t shape;
  for (const auto& dim : vi.type().tensor_type().shape().dim()) {
    CHECK(dim.has_dim_value());
    shape.push_back(dim.dim_value());
  }
}

OnnxMeta genOnnxMeta(const ::ONNX_NAMESPACE::ModelProto& model) {
  OnnxMeta meta;
  auto& nodes = model.graph().node();
  auto& initializers = model.graph().initializer();
  auto& value_infos = model.graph().value_info();
  auto& inputs = model.graph().input();
  auto& outputs = model.graph().output();

  // parse inputs
  for (const auto& input : inputs) {
    meta.inputs.emplace_back(input.name());
    empty_and_insert(meta.name_to_shape_, input.name(),
                     handleValueInfoProtoToShape(input));
  }
  // parse outputs
  for (const auto& output : outputs) {
    meta.outputs.emplace_back(output.name());
    empty_and_insert(meta.name_to_shape_, output.name(),
                     handleValueInfoProtoToShape(output));
  }
  // parse initializers
  for (size_t i = 0; i < initializers.size(); i++) {
    auto& init = initializers[i];
    empty_and_insert(meta.name_to_initializer_idx_, init.name(), i);
    auto& dims = init.dims();
    auto dimsv = std::vector<int64_t>(dims.begin(), dims.end());
    empty_and_insert(meta.name_to_shape_, init.name(), dimsv);
  }
  // parse node
  for (size_t i = 0; i < nodes.size(); i++) {
    auto& node = nodes[i];
    empty_and_insert(meta.name_to_node_idx_, node.name(), i);
    for (size_t j = 0; j < node.input().size(); j++) {
      auto input = node.input()[j];
      // add name_as_node_input_
      empty_and_insert(meta.name_as_node_input_, input, node.name());
    }
    for (size_t j = 0; j < node.output().size(); j++) {
      auto output = node.output()[j];
      // add name_as_node_output_
      empty_and_insert(meta.name_as_node_output_, output, node.name());
    }
  }
  // parse value_info
  for (const auto& vi : value_infos) {
    empty_and_insert(meta.name_to_shape_, vi.name(),
                     handleValueInfoProtoToShape(vi));
  }
  return meta;
}

}  // namespace simonnx
}  // namespace utils
