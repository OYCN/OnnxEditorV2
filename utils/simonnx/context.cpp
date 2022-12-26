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
#include <filesystem>
#include <fstream>
#include <thread>  // NOLINT [build/c++11]
#include <utility>

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

namespace utils {
namespace simonnx {

namespace fs = std::filesystem;

#define LOCK                                           \
  do {                                                 \
    VLOG(1) << "lock @" << std::this_thread::get_id(); \
    std::unique_lock lock(mutex_);                     \
  } while (0)

SimOnnxCtx::SimOnnxCtx() {
  mp_ = new ::ONNX_NAMESPACE::ModelProto();
  resetDebugFn();
  resetInfoFn();
  resetErrorFn();
}
SimOnnxCtx::~SimOnnxCtx() { delete mp_; }

NodeHandle SimOnnxCtx::CreateNewNodeObj() {
  LOCK;
  CHECK_NOTNULL(mp_);
  auto nodes = mp_->mutable_graph()->mutable_node();
  auto node_ptr = nodes->Add();
  return CreateNodeObjImpl(node_ptr);
}

void SimOnnxCtx::DeleteObj(IObject* obj) {
  LOCK;
  if (obj->isDeleted() == false) {
    obj->setDeleted(true);
    auto objtype = obj->getObjType();
    obj_free_ctx_[objtype].erase(obj->getIter());
    obj_del_ctx_[objtype].emplace_back(obj);
    obj->setIter(std::prev(obj_del_ctx_[objtype].end()));
  }
}

void SimOnnxCtx::RestoreObj(IObject* obj) {
  LOCK;
  if (obj->isDeleted() == true) {
    obj->setDeleted(false);
    auto objtype = obj->getObjType();
    obj_del_ctx_[objtype].erase(obj->getIter());
    obj_free_ctx_[objtype].emplace_back(obj);
    obj->setIter(std::prev(obj_free_ctx_[objtype].end()));
  }
}

void SimOnnxCtx::genRandomOnnx(int num) {
  LOCK;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  reset_impl();
  auto g = utils::algorithm::external::ogdf::genRandomGraph(num, num);

  size_t edge_count = 0;
  std::map<std::pair<size_t, size_t>, std::string> tensor_map;
  for (size_t i = 0; i < g.getLen(); i++) {
    for (auto j : g.getOutput(i)) {
      auto nn = std::make_pair(i, j);
      if (tensor_map.count(nn) == 0) {
        auto name = std::to_string(edge_count++);
        FakeTensor_t args = {name};
        SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(args);
        tensor_map[nn] = name;
      }
    }
  }

  for (size_t i = 0; i < g.getLen(); i++) {
    std::vector<std::string> input;
    for (auto j : g.getInput(i)) {
      auto nn = std::make_pair(j, i);
      CHECK_EQ(tensor_map.count(nn), 1);
      input.emplace_back(tensor_map.at(nn));
    }
    std::vector<std::string> output;
    for (auto j : g.getOutput(i)) {
      auto nn = std::make_pair(i, j);
      CHECK_EQ(tensor_map.count(nn), 1);
      output.emplace_back(tensor_map.at(nn));
    }
    FakeNode_t args = {std::to_string(i), std::to_string(i), input, output};
    SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(args);
  }
}

bool SimOnnxCtx::openOnnx(const std::string path) {
  LOCK;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  reset_impl();
  if (!fs::exists(path)) {
    errorfn_("Path invalid " + path);
    return false;
  }
  if (fs::is_directory(path)) {
    errorfn_("Path is not a file " + path);
    return false;
  }
  std::ifstream fin(path, std::ios::in | std::ios::binary);
  if (!fin.good()) {
    errorfn_("Could not open file " + path);
    return false;
  }
  if (!mp_->ParseFromIstream(&fin)) {
    errorfn_("proto parse error");
    return false;
  }
  std::set<std::string> tensor_set;
  auto create = [&](auto proto_ptr) {
    if (tensor_set.count(proto_ptr->name()) == 0) {
      tensor_set.insert(proto_ptr->name());
      SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(proto_ptr);
      return true;
    } else {
      return false;
    }
  };
  auto& proto_nodes = mp_->graph().node();
  auto& proto_initializers = mp_->graph().initializer();
  auto& proto_inputs = mp_->graph().input();
  auto& proto_outputs = mp_->graph().output();
  auto& proto_value = mp_->graph().value_info();
  for (size_t i = 0; i < proto_nodes.size(); i++) {
    auto& node = proto_nodes[i];
    auto node_ptr = mp_->mutable_graph()->mutable_node(i);
    SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(node_ptr);
  }
  for (size_t i = 0; i < proto_initializers.size(); i++) {
    auto init_ptr = mp_->mutable_graph()->mutable_initializer(i);
    if (create(init_ptr)) {
      FakeNode_t args = {"::initializer::", "", {}, {init_ptr->name()}};
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(args);
    }
  }
  int input_num = 0;
  for (size_t i = 0; i < proto_inputs.size(); i++) {
    auto input_ptr = mp_->mutable_graph()->mutable_input(i);
    if (create(input_ptr)) {
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(input_ptr,
                                                 NodeObj::kInputNode);
      input_num++;
    }
  }
  for (size_t i = 0; i < proto_outputs.size(); i++) {
    auto output_ptr = mp_->mutable_graph()->mutable_output(i);
    if (create(output_ptr)) {
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(output_ptr,
                                                 NodeObj::kOutputNode);
    }
  }
  for (size_t i = 0; i < proto_value.size(); i++) {
    auto value_ptr = mp_->mutable_graph()->mutable_value_info(i);
    create(value_ptr);
  }

  LOG(INFO) << " === Graph Summary After Read === ";
  LOG(INFO) << "\t Input num:\t" << input_num;
  LOG(INFO) << "\t Output num:\t" << proto_outputs.size();
  LOG(INFO) << "\t Node num:\t" << proto_nodes.size();
  LOG(INFO) << "\t Initializer num:\t" << proto_initializers.size();
  LOG(INFO) << "\t ValueInfo num:\t" << proto_value.size();
  return true;
}

bool SimOnnxCtx::saveOnnx(const std::string path, bool overwrite) {
  LOCK;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (fs::exists(path) && !overwrite) {
    errorfn_("Path already exist " + path);
    return false;
  }
  if (fs::is_directory(path)) {
    errorfn_("Path is a directory" + path);
    return false;
  }
  std::ofstream fout(path, std::ios::out | std::ios::binary);
  if (!fout.good()) {
    errorfn_("Could not open file " + path);
    return false;
  } else {
    bool ret = mp_->SerializeToOstream(&fout);
    return ret;
  }
}

void SimOnnxCtx::reset_impl() {
  LOG(INFO) << "SimOnnxCtx reset: " << this;
  if (obj_free_ctx_.size() > 0) {
    for (auto kv : obj_free_ctx_) {
      for (auto ptr : obj_free_ctx_.at(kv.first)) {
        delete ptr;
      }
      obj_free_ctx_.at(kv.first).clear();
    }
  }
  if (obj_del_ctx_.size() > 0) {
    for (auto kv : obj_del_ctx_) {
      for (auto ptr : obj_del_ctx_.at(kv.first)) {
        delete ptr;
      }
      obj_del_ctx_.at(kv.first).clear();
    }
  }
}
void SimOnnxCtx::reset() {
  LOCK;
  reset_impl();
}

void SimOnnxCtx::setDebugFn(std::function<void(std::string)> fn) {
  LOCK;
  debugfn_ = fn;
}
void SimOnnxCtx::setInfoFn(std::function<void(std::string)> fn) {
  LOCK;
  infofn_ = fn;
}
void SimOnnxCtx::setErrorFn(std::function<void(std::string)> fn) {
  LOCK;
  errorfn_ = fn;
}
void SimOnnxCtx::resetDebugFn() {
  LOCK;
  debugfn_ = [](std::string msg) { VLOG(1) << msg; };
}
void SimOnnxCtx::resetInfoFn() {
  LOCK;
  infofn_ = [](std::string msg) { LOG(INFO) << msg; };
}
void SimOnnxCtx::resetErrorFn() {
  LOCK;
  errorfn_ = [](std::string msg) { LOG(ERROR) << msg; };
}

}  // namespace simonnx
}  // namespace utils
