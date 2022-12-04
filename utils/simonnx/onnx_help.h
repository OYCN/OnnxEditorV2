/**
 * @file onnx_help.h
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

#ifndef UTILS_SIMONNX_ONNX_HELP_H_
#define UTILS_SIMONNX_ONNX_HELP_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "utils/algorithm/graph_desc.h"
#include "utils/simonnx/context.h"

namespace utils {
namespace simonnx {

using NodeHandle = utils::simonnx::NodeHandle;
using TensorHandle = utils::simonnx::TensorHandle;
using GraphNode2NodeDescExtTmp =
    utils::algorithm::desc::GraphNode2NodeDescExtTmp<NodeHandle, TensorHandle>;

GraphNode2NodeDescExtTmp onnx2graph(SimOnnxCtx* ctx);
GraphNode2NodeDescExtTmp onnx2graph(const std::string path);
GraphNode2NodeDescExtTmp gen_random_graph(int num);
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_ONNX_HELP_H_
