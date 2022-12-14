/**
 * @file backend.h
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

#ifndef UTILS_SIMONNX_BACKEND_BACKEND_H_
#define UTILS_SIMONNX_BACKEND_BACKEND_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace utils {
namespace simonnx {
namespace backend {

class IBackendCtx;
class IBackendGraph;
class IBackendNode;
class IBackendValueInfo;
class IBackendTensor;
class IBackendAttribute;

using SBackendCtx = std::shared_ptr<IBackendCtx>;
using SBackendGraph = std::shared_ptr<IBackendGraph>;
using SBackendNode = std::shared_ptr<IBackendNode>;
using SBackendValueInfo = std::shared_ptr<IBackendValueInfo>;
using SBackendTensor = std::shared_ptr<IBackendTensor>;
using SBackendAttribute = std::shared_ptr<IBackendAttribute>;

enum class BackendType { kProto, kOnnxIR };

enum class PassType {
  kNone,
  kAll,
  kTopoSort
};

class IBackendCtx {
 public:
  static std::shared_ptr<IBackendCtx> createCtx(BackendType type);

 public:
  void setSptr(std::shared_ptr<IBackendCtx> sptr) { wptr_ = sptr; }
  std::shared_ptr<IBackendCtx> getSPtr() { return wptr_.lock(); }
  virtual bool loadFile(const std::string& path) = 0;
  virtual bool saveFile(const std::string& path, bool overwrite = true) = 0;
  virtual SBackendGraph graph() const = 0;
  virtual bool pass(PassType type) = 0;

 private:
  std::weak_ptr<IBackendCtx> wptr_;
};

class IBackendGraph {
 public:
  virtual bool destroy() = 0;
  virtual const std::vector<SBackendNode>& node() const = 0;
  virtual SBackendNode add_node() = 0;
  virtual bool del_node(SBackendNode node) = 0;
  virtual const std::vector<SBackendValueInfo>& input() const = 0;
  virtual SBackendValueInfo add_input() = 0;
  virtual bool del_input(SBackendValueInfo input) = 0;
  virtual const std::vector<SBackendValueInfo>& output() const = 0;
  virtual SBackendValueInfo add_output() = 0;
  virtual bool del_output(SBackendValueInfo value_info) = 0;
  virtual const std::vector<SBackendTensor>& initializer() const = 0;
  virtual SBackendTensor add_initializer() = 0;
  virtual bool del_initializer(SBackendTensor tensor) = 0;
  virtual const std::vector<SBackendValueInfo>& value_info() const = 0;
  virtual SBackendValueInfo add_value_info() = 0;
  virtual bool del_value_info(SBackendValueInfo value_info) = 0;
};

class IBackendNode {
 public:
  virtual bool destroy() = 0;
  virtual std::string name() const = 0;
  virtual bool set_name(const std::string& name) = 0;
  virtual std::string op_type() const = 0;
  virtual bool set_op_type(const std::string& op_type) = 0;
  virtual std::vector<std::string> input() const = 0;
  virtual bool set_input(const std::vector<std::string>& input) = 0;
  virtual std::vector<std::string> output() const = 0;
  virtual bool set_output(const std::vector<std::string>& output) = 0;
  virtual const std::vector<SBackendAttribute>& attribute() const = 0;
  virtual SBackendAttribute add_attribute() = 0;
  virtual bool del_attribute(SBackendAttribute attr) = 0;
};

class IBackendValueInfo {
 public:
  virtual bool destroy() = 0;
  virtual std::string name() const = 0;
  virtual bool set_name(const std::string& name) = 0;
  virtual std::string type() const = 0;
  virtual bool set_type(const std::string& type) = 0;
  virtual std::vector<int64_t> dim() const = 0;
  virtual bool set_dim(const std::vector<int64_t>& dim) = 0;
};

class IBackendTensor {
 public:
  virtual bool destroy() = 0;
  virtual std::string name() const = 0;
  virtual bool set_name(const std::string& name) = 0;
  virtual std::string type() const = 0;
  virtual bool set_type(const std::string& type) = 0;
  virtual std::vector<int64_t> dim() const = 0;
  virtual bool set_dim(const std::vector<int64_t>& dim) = 0;
  virtual void* data() const = 0;
  virtual size_t data_size() const = 0;
  virtual bool set_data(void* ptr, size_t len) = 0;
};

class IBackendAttribute {
 public:
  virtual std::string name() const = 0;
  virtual bool set_name(const std::string& name) = 0;
};

}  // namespace backend
}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_BACKEND_BACKEND_H_
