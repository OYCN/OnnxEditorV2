/**
 * @file onnx.h
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

#ifndef UTILS_ONNX_ONNX_H_
#define UTILS_ONNX_ONNX_H_

#include <list>
#include <map>
#include <string>
#include <vector>

namespace utils {
namespace onnx {

struct SimOnnx {
  enum class DataType_t { kNone, kFloat64, kInt64, kUnHandled, kMax };

  using Shape_t = std::vector<int64_t>;

  struct Tensor_t {
    std::string name;
    Shape_t shape;
    DataType_t type;
    void* data;
    std::list<std::string> as_node_inputs;
    std::list<std::string> as_node_outputs;
  };

  struct Node_t {
    struct Attr_t {
      enum class DataType_t {
        kNone,
        kStr,
        kInt64,
        kFloat64,
        kTensor,
        kUnHandled,
        kMax
      };
      std::string name;
      DataType_t type;
      void* data;
    };
    std::string name;
    std::string op_type;
    std::list<std::string> inputs;
    std::list<std::string> outputs;
  };

  std::list<Node_t> nodes;
  std::list<Tensor_t> initializers;
  std::list<Tensor_t> inputs;
  std::list<Tensor_t> outputs;
  std::list<Tensor_t> activations;

  bool is_valid = false;
  std::map<std::string, size_t> node_name_idx_map;
  std::map<std::string, Tensor_t*> tensor_name_ptr_map;
};

}  // namespace onnx
}  // namespace utils

#endif  // UTILS_ONNX_ONNX_H_
