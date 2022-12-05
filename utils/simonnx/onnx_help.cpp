/**
 * @file onnx_help.cpp
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

#include "utils/simonnx/onnx_help.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include <magic_enum.hpp>

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

template <typename _K, typename _V>
void map_set_unique_insert(std::map<_K, std::set<_V>>& mapset, const _K& key,
                           const _V& val) {
  if (mapset.count(key) == 1) {
    CHECK_EQ(mapset.at(key).count(val), 0) << key;
  } else {
    mapset[key].insert(val);
  }
}

GraphNode2NodeDescExtTmp onnx2graph(SimOnnxCtx* ctx) {
  auto& model = *SimOnnxCtx::getSimOnnxCtx()->getModelProtoPtr();
  // base desc
  size_t len = model.graph().node().size();
  std::vector<std::vector<size_t>> out_outputs(len);
  std::vector<std::vector<size_t>> out_inputs(len);
  std::vector<size_t> out_roots(len);
  std::vector<std::pair<float, float>> out_whs(len, {0, 0});
  // ext desc
  std::vector<NodeHandle> out_id2node(len);
  std::map<std::pair<size_t, size_t>, TensorHandle> out_node2tensor;
  // tmp var
  std::set<std::string> graph_inputs;
  std::set<std::string> graph_outputs;
  std::map<std::string, std::set<size_t>> graph_input_map_node;
  std::map<std::string, std::set<size_t>> graph_output_map_node;
  std::map<std::string, std::set<size_t>> graph_init_map_dix;
  std::map<std::string, std::set<size_t>> graph_value_map_dix;
  auto& proto_nodes = model.graph().node();
  auto& proto_initializers = model.graph().initializer();
  auto& proto_inputs = model.graph().input();
  auto& proto_outputs = model.graph().output();
  auto& proto_value = model.graph().value_info();
  // collect onnx meta
  for (size_t i = 0; i < len; i++) {
    auto& node = proto_nodes[i];
    for (const auto& input : node.input()) {
      map_set_unique_insert(graph_input_map_node, input, i);
    }
    for (const auto& output : node.output()) {
      map_set_unique_insert(graph_output_map_node, output, i);
    }
  }
  for (size_t i = 0; i < proto_initializers.size(); i++) {
    auto& init = proto_initializers[i];
    VLOG(1) << "parse initializers name: " << init.name();
    map_set_unique_insert(graph_init_map_dix, init.name(), i);
  }
  for (size_t i = 0; i < proto_inputs.size(); i++) {
    auto& input = proto_inputs[i].name();
    CHECK_EQ(graph_inputs.count(input), 0) << input;
    if (graph_init_map_dix.count(input) == 0) {
      graph_inputs.insert(input);
    }
  }
  for (size_t i = 0; i < proto_outputs.size(); i++) {
    auto& output = proto_outputs[i].name();
    CHECK_EQ(graph_outputs.count(output), 0) << output;
    graph_outputs.insert(output);
  }
  for (size_t i = 0; i < proto_value.size(); i++) {
    auto& value = proto_value[i];
    VLOG(1) << "parse value info name: " << value.name();
    map_set_unique_insert(graph_value_map_dix, value.name(), i);
  }
  // gen output var
  for (size_t i = 0; i < len; i++) {
    auto& node = proto_nodes[i];
    auto& name = node.name();
    auto node_ptr = model.mutable_graph()->mutable_node(i);
    out_id2node[i] = SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(node_ptr);
    for (const auto& input : node.input()) {
      if (graph_output_map_node.count(input) == 1) {
        auto& target_nodes = graph_output_map_node.at(input);
        CHECK_EQ(target_nodes.size(), 1) << input;
        auto target_node_idx = *target_nodes.begin();
        out_inputs[i].push_back(target_node_idx);
        std::pair<size_t, size_t> nn(target_node_idx, i);
        CHECK_EQ(out_node2tensor.count(nn), 0) << input;
        VLOG(1) << "add edge: " << target_node_idx << " -> " << i;
        if (graph_value_map_dix.count(input) != 0) {
          auto value_ptr = model.mutable_graph()->mutable_value_info(i);
          out_node2tensor[nn] =
              SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(value_ptr);
        } else if (graph_init_map_dix.count(input) != 0) {
          auto init_ptr = model.mutable_graph()->mutable_initializer(i);
          out_node2tensor[nn] =
              SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(init_ptr);
        } else {
          FakeTensor_t args = {input};
          out_node2tensor[nn] =
              SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(args);
        }
      } else if (graph_inputs.count(input)) {
        out_roots.emplace_back(i);
      } else {
        CHECK_EQ(graph_init_map_dix.count(input), 1) << input;
      }
    }
    for (const auto& output : node.output()) {
      if (graph_input_map_node.count(output) != 0) {
        for (const auto& target_node : graph_input_map_node.at(output)) {
          out_outputs[i].push_back(target_node);
        }
      }
    }
  }
  return GraphNode2NodeDescExtTmp(
      {len, out_outputs, out_inputs, out_whs, out_roots}, out_id2node,
      out_node2tensor, SimOnnxCtx::getSimOnnxCtx());
}

GraphNode2NodeDescExtTmp onnx2graph(const std::string path) {
  SimOnnxCtx::getSimOnnxCtx()->openOnnx(path);
  return onnx2graph(SimOnnxCtx::getSimOnnxCtx());
}

using NodeProto = ::ONNX_NAMESPACE::NodeProto;
GraphNode2NodeDescExtTmp gen_random_graph(int num) {
  auto g = utils::algorithm::external::ogdf::genRandomGraph(num, num);

  std::vector<NodeHandle> idx2node(g.getLen());
  for (size_t i = 0; i < g.getLen(); i++) {
    FakeNode_t args = {std::to_string(i), std::to_string(i)};
    idx2node[i] = SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(args);
  }

  size_t edge_count = 0;
  std::map<std::pair<size_t, size_t>, TensorHandle> node2tensor;
  for (size_t i = 0; i < g.getLen(); i++) {
    for (auto j : g.getOutput(i)) {
      FakeTensor_t args = {std::to_string(edge_count++)};
      node2tensor[{i, j}] = SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(args);
    }
  }

  return GraphNode2NodeDescExtTmp(g, idx2node, node2tensor,
                                  SimOnnxCtx::getSimOnnxCtx());
}

}  // namespace simonnx
}  // namespace utils
