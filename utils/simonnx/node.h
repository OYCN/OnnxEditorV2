/**
 * @file node.h
 * @brief 
 * @author opluss (opluss@qq.com)
 * 
 * @copyright Copyright (c) 2022  opluss
 * 
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-03 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_SIMONNX_NODE_H_
#define UTILS_SIMONNX_NODE_H_

#include <functional>
#include <list>
#include <string>
#include <vector>

#include "utils/simonnx/object.h"

namespace utils {
namespace simonnx {

using NodeObjBase = Object<ObjType_t::kNode>;

class NodeObj : public NodeObjBase {
 public:
  explicit NodeObj(std::string name = "", std::string op_type = "")
      : NodeObjBase(), name_(name), op_type_(op_type) {}
  ~NodeObj() {}

  std::string getName() { return name_; }
  void setName(std::string name) { name_ = name; }
  std::string getOpType() { return op_type_; }
  void setOpType(std::string op_type) { op_type_ = op_type; }

 private:
  std::string name_;
  std::string op_type_;
};

using NodeHandle = utils::simonnx::NodeObj*;

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_NODE_H_
