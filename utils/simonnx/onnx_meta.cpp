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

#include <filesystem>
#include <fstream>
#include <magic_enum.hpp>

#define CHECK_RETURN(cond, def) \
  do {                          \
    bool ret = (cond);          \
    if (!ret) {                 \
      return def;               \
    }                           \
  } while (0)

namespace utils {
namespace simonnx {

namespace fs = std::filesystem;

::ONNX_NAMESPACE::ModelProto open_onnx(const std::string path) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  CHECK(fs::exists(path));
  CHECK(!fs::is_directory(path));
  ::ONNX_NAMESPACE::ModelProto OnnxModel;
  std::ifstream fin(path, std::ios::in | std::ios::binary);
  if (!fin.good()) {
    LOG(FATAL) << "Could not open file " << path;
    return OnnxModel;
  }
  bool ret = OnnxModel.ParseFromIstream(&fin);
  CHECK(ret);
  return OnnxModel;
}

template <typename _K, typename _V>
void map_insert(std::map<_K, _V>& map, const _K& key, const _V& val) {
  // CHECK_EQ(map.count(key), 0) << "Name of " << key << " has been inserted";
  if (map.count(key) == 1) {
    CHECK(map.at(key) == val) << key;
  } else {
    map[key] = val;
  }
}

template <typename _K, typename _V>
void map_set_insert(std::map<_K, std::set<_V>>& mapset, const _K& key, const _V& val) {
  // CHECK_EQ(map.count(key), 0) << "Name of " << key << " has been inserted";
  mapset[key].insert(val);
}

OnnxMeta::Shape_t handleValueInfoProtoToShape(
    const ::ONNX_NAMESPACE::ValueInfoProto& vi) {
  VLOG(1) << "parse ValueInfoProto: " << vi.name();
  CHECK_RETURN(vi.has_type() && vi.type().has_tensor_type() &&
                   vi.type().tensor_type().has_shape(),
               {});
  OnnxMeta::Shape_t shape;
  for (const auto& dim : vi.type().tensor_type().shape().dim()) {
    if (dim.has_dim_value()) {
      shape.push_back(dim.dim_value());
    } else {
      // TODO(opluss): may be a string means dynamic dim
      shape.push_back(-1);
    }
  }
  CHECK_EQ(vi.type().tensor_type().shape().dim().size(), shape.size());
  return shape;
}

OnnxMeta genOnnxMeta(const ::ONNX_NAMESPACE::ModelProto& model) {
  OnnxMeta meta;
  auto& nodes = model.graph().node();
  auto& initializers = model.graph().initializer();
  auto& value_infos = model.graph().value_info();
  auto& inputs = model.graph().input();
  auto& outputs = model.graph().output();

  // parse initializers
  LOG(INFO) << "parse initializers num : " << initializers.size();
  for (size_t i = 0; i < initializers.size(); i++) {
    auto& init = initializers[i];
    VLOG(1) << "parse initializers name: " << init.name();
    map_insert(meta.name_to_initializer_idx_, init.name(), i);
    auto& dims = init.dims();
    auto dimsv = std::vector<int64_t>(dims.begin(), dims.end());
    map_insert(meta.name_to_shape_, init.name(), dimsv);
    meta.inits.insert(init.name());
  }
  // parse inputs
  LOG(INFO) << "parse inputs num: " << inputs.size();
  for (const auto& input : inputs) {
    if (meta.inits.count(input.name()) == 0 ) {
      meta.inputs.insert(input.name());
      map_insert(meta.name_to_shape_, input.name(),
                      handleValueInfoProtoToShape(input));
    }
  }
  // parse outputs
  LOG(INFO) << "parse outputs num: " << outputs.size();
  for (const auto& output : outputs) {
    meta.outputs.insert(output.name());
    map_insert(meta.name_to_shape_, output.name(),
                     handleValueInfoProtoToShape(output));
  }
  // parse node
  LOG(INFO) << "parse node num: " << nodes.size();
  for (size_t i = 0; i < nodes.size(); i++) {
    auto& node = nodes[i];
    VLOG(1) << "parse node name: " << node.name();
    VLOG(1) << "insert name_to_node_idx_";
    map_insert(meta.name_to_node_idx_, node.name(), i);
    for (size_t j = 0; j < node.input().size(); j++) {
      auto input = node.input()[j];
      // add name_as_node_input_
      VLOG(1) << "insert name_as_node_input_";
      map_set_insert(meta.name_as_node_input_, input, node.name());
    }
    for (size_t j = 0; j < node.output().size(); j++) {
      auto output = node.output()[j];
      // add name_as_node_output_
      VLOG(1) << "insert name_as_node_output_";
      map_insert(meta.name_as_node_output_, output, node.name());
    }
  }
  // parse value_info
  LOG(INFO) << "parse value_info num: " << value_infos.size();
  for (const auto& vi : value_infos) {
    map_insert(meta.name_to_shape_, vi.name(),
                     handleValueInfoProtoToShape(vi));
  }
  LOG(INFO) << " === Onnx Summary === ";
  LOG(INFO) << "\t inputs num:\t" << inputs.size();
  LOG(INFO) << "\t outputs num:\t" << outputs.size();
  LOG(INFO) << "\t initializers num:\t" << initializers.size();
  LOG(INFO) << "\t value_infos num:\t" << value_infos.size();
  LOG(INFO) << "\t nodes num:\t" << nodes.size();

  LOG(INFO) << " === OnnxMeta Summary === ";
  LOG(INFO) << "\t inputs num:\t" << meta.inputs.size();
  LOG(INFO) << "\t outputs num:\t" << meta.outputs.size();
  LOG(INFO) << "\t inits num:\t" << meta.inits.size();
  LOG(INFO) << "\t name_to_node_idx_ num:\t" << meta.name_to_node_idx_.size();
  LOG(INFO) << "\t name_to_initializer_idx_ num:\t" << meta.name_to_initializer_idx_.size();
  LOG(INFO) << "\t name_as_node_input_ num:\t" << meta.name_as_node_input_.size();
  LOG(INFO) << "\t name_as_node_output_ num:\t" << meta.name_as_node_output_.size();
  LOG(INFO) << "\t name_to_shape_ num:\t" << meta.name_to_shape_.size();
  return meta;
}

OnnxMeta genOnnxMeta(const std::string path) {
  return genOnnxMeta(open_onnx(path));
}

template <typename _T>
std::string cvtAttr1DNums(std::function<_T(size_t i)> f, size_t len, bool array, std::string prefix) {
  if (!array) {
    CHECK_EQ(len, 1);
    return prefix + std::to_string(f(0));
  } else {
    std::string out;
    out += "[";
    for (size_t i = 0; i < len; i++) {
      out += std::to_string(f(i)) + ",";
    }
    out += "]";
    return prefix + out;
  }
}

std::string cvtAttributeProtoValue2Str(
    const ::ONNX_NAMESPACE::AttributeProto& attrpo) {
  CHECK(attrpo.has_type());
  switch (attrpo.type()) {
    case ::ONNX_NAMESPACE::AttributeProto_AttributeType::
        AttributeProto_AttributeType_FLOAT: {
      CHECK(attrpo.has_f());
      return cvtAttr1DNums<float>([&](size_t i){return attrpo.f();}, 1, false, "");
    }
    case ::ONNX_NAMESPACE::AttributeProto_AttributeType::
        AttributeProto_AttributeType_INT: {
      CHECK(attrpo.has_i());
      return cvtAttr1DNums<int32_t>([&](size_t i){return attrpo.i();}, 1, false, "");
    }
    case ::ONNX_NAMESPACE::AttributeProto_AttributeType::
        AttributeProto_AttributeType_STRING: {
      CHECK(attrpo.has_s());
      return "\"" + attrpo.s() + "\"";
    }
    case ::ONNX_NAMESPACE::AttributeProto_AttributeType::
        AttributeProto_AttributeType_FLOATS: {
      return cvtAttr1DNums<float>([&](size_t i){return attrpo.floats(i);}, attrpo.floats_size(), true, "");
    }
    case ::ONNX_NAMESPACE::AttributeProto_AttributeType::
        AttributeProto_AttributeType_INTS: {
      return cvtAttr1DNums<int32_t>([&](size_t i){return attrpo.ints(i);}, attrpo.ints_size(), true, "");
    }
    default:
      LOG(FATAL) << "Unsupport onnx node attr type: "
                 << magic_enum::enum_name(attrpo.type());
  }
}

GraphNode2NodeDescExtTmp onnx2graph(const ::ONNX_NAMESPACE::ModelProto& model,
                                    OnnxMeta* _meta) {
  OnnxMeta meta;
  if (_meta == nullptr) {
    meta = genOnnxMeta(model);
  } else {
    meta = *_meta;
  }

  // base desc
  size_t len = model.graph().node().size();
  std::vector<std::vector<size_t>> outputs(len);
  std::vector<std::vector<size_t>> inputs(len);
  std::vector<std::pair<float, float>> whs(len, {0, 0});
  std::vector<size_t> roots;
  // ext desc
  std::vector<std::vector<std::string>> node2name(len);
  std::vector<std::vector<std::pair<std::string, std::string>>> node2attrs(len);
  std::map<std::pair<size_t, size_t>, std::string> edge2name;
  for (size_t i = 0; i < len; i++) {
    auto& node = model.graph().node()[i];
    auto& name = node.name();
    node2name[i] = {name, node.op_type()};
    for (const auto& input : node.input()) {
      if (meta.name_as_node_output_.count(input) == 1) {
        auto& target_node = meta.name_as_node_output_.at(input);
        CHECK_EQ(meta.name_to_node_idx_.count(target_node), 1) << target_node;
        auto& target_idx = meta.name_to_node_idx_.at(target_node);
        inputs[i].push_back(target_idx);
        std::pair<size_t, size_t> nn(target_idx, i);
        CHECK_EQ(edge2name.count(nn), 0);
        VLOG(1) << "add edge: " << target_idx << " -> " << i;
        edge2name[nn] = input;
      } else {
         CHECK_EQ(meta.inputs.count(input) + meta.inits.count(input), 1) << input;
      }
    }
    if (inputs[i].size() == 0) {
      roots.push_back(i);
    }
    for (const auto& output : node.output()) {
      if (meta.name_as_node_input_.count(output) != 0) {
        for (const auto& target_node : meta.name_as_node_input_.at(output)) {
          CHECK_EQ(meta.name_to_node_idx_.count(target_node), 1) << target_node;
          auto& target_idx = meta.name_to_node_idx_.at(target_node);
          outputs[i].push_back(target_idx);
        }
      }
    }
    for (const auto& attribute : node.attribute()) {
      std::string value = cvtAttributeProtoValue2Str(attribute);
      CHECK(attribute.has_name());
      std::pair<std::string, std::string> attr(attribute.name(), value);
      node2attrs[i].push_back(attr);
    }
  }
  return GraphNode2NodeDescExtTmp({len, outputs, inputs, whs, roots}, node2name,
                                  node2attrs, edge2name);
}

GraphNode2NodeDescExtTmp onnx2graph(const std::string path, OnnxMeta* _meta) {
  return onnx2graph(open_onnx(path), _meta);
}

}  // namespace simonnx
}  // namespace utils
