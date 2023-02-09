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

#include <cstdio>
#include <set>
#include <thread>  // NOLINT [build/c++11]
#include <utility>

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"
#include "utils/simonnx/backend/backend.h"

#define CHECK_BINDED                              \
  do {                                            \
    if (!binded()) {                              \
      LOG(WARNING) << "SimOnnxCtx not be binded"; \
    }                                             \
  } while (0)
namespace utils {
namespace simonnx {

#define LOCK                                           \
  do {                                                 \
    VLOG(1) << "lock @" << std::this_thread::get_id(); \
    std::unique_lock lock(mutex_);                     \
  } while (0)

SimOnnxCtx::SimOnnxCtx(backend::BackendType type) {
  bkctx_ = backend::IBackendCtx::createCtx(type);
  CHECK(bkctx_);
}
SimOnnxCtx::~SimOnnxCtx() {}

NodeHandle SimOnnxCtx::CreateNewNodeObj() {
  LOCK;
  CHECK_BINDED;
  CHECK_NOTNULL(bkctx_);
  auto graph = bkctx_->graph();
  CHECK_NOTNULL(graph);
  auto node_ptr = graph->add_node();
  CHECK_NOTNULL(node_ptr);
  return CreateNodeObjImpl(node_ptr);
}

NodeHandle SimOnnxCtx::CreateNewIOObj(IONodeType type) {
  LOCK;
  CHECK_BINDED;
  CHECK_NOTNULL(bkctx_);
  auto graph = bkctx_->graph();
  CHECK_NOTNULL(graph);
  SBackendValueInfo io_ptr = nullptr;
  switch (type) {
    case IONodeType::kInputNode:
      io_ptr = graph->add_input();
      break;
    case IONodeType::kOutputNode:
      io_ptr = graph->add_output();
      break;
    default:
      LOG(FATAL) << "type error";
      break;
  }
  CHECK_NOTNULL(io_ptr);
  return CreateNodeObjImpl(io_ptr, type);
}

void SimOnnxCtx::DeleteObj(IObject* obj) {
  LOCK;
  CHECK_BINDED;
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
  CHECK_BINDED;
  if (obj->isDeleted() == true) {
    obj->setDeleted(false);
    auto objtype = obj->getObjType();
    obj_del_ctx_[objtype].erase(obj->getIter());
    obj_free_ctx_[objtype].emplace_back(obj);
    obj->setIter(std::prev(obj_free_ctx_[objtype].end()));
  }
}

bool SimOnnxCtx::binded() {
  LOCK;
  return owner_ != nullptr;
}
bool SimOnnxCtx::trybindby(void* ptr) {
  LOCK;
  if (owner_ != nullptr || ptr == nullptr) {
    return false;
  }
  owner_ = ptr;
  return true;
}
bool SimOnnxCtx::tryunbindby(void* ptr) {
  LOCK;
  if (ptr == nullptr || owner_ != ptr) {
    return false;
  }
  owner_ = nullptr;
  return true;
}
void SimOnnxCtx::bindby(void* ptr) { CHECK(trybindby(ptr)); }
void SimOnnxCtx::unbindby(void* ptr) { CHECK(tryunbindby(ptr)); }

void SimOnnxCtx::genRandomOnnx(int num) {
  LOCK;
  CHECK_BINDED;
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
  CHECK_BINDED;
  reset_impl();
  if (!bkctx_->loadFile(path)) {
    return false;
  }
  std::set<std::string> name_set;
  auto graph = bkctx_->graph();
  CHECK_NOTNULL(graph);
  auto nodes = graph->node();
  auto inits = graph->initializer();
  auto inputs = graph->input();
  auto outputs = graph->output();
  auto value_infoes = graph->value_info();
  for (auto n : nodes) {
    SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(n);
  }
  for (auto i : inits) {
    if (name_set.count(i->name()) == 0) {
      FakeNode_t args = {TREATY_INIT_OP_TYPE, "", {}, {i->name()}};
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(args);
      name_set.insert(i->name());
      VLOG(1) << "inits add: " << i->name();
    } else {
      LOG(ERROR) << "duplicated name in inits: " << i->name();
    }
  }
  int input_num = 0;
  for (auto i : inputs) {
    if (name_set.count(i->name()) == 0) {
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(i, NodeObj::kInputNode);
      name_set.insert(i->name());
      input_num++;
      VLOG(1) << "inputs add: " << i->name();
    } else {
      LOG(ERROR) << "duplicated name in inputs: " << i->name();
    }
  }
  for (auto o : outputs) {
    if (name_set.count(o->name()) == 0) {
      name_set.insert(o->name());
      SimOnnxCtx::getSimOnnxCtx()->CreateNodeObj(o, NodeObj::kOutputNode);
      VLOG(1) << "outputs add: " << o->name();
    } else {
      LOG(ERROR) << "duplicated name in outputs: " << o->name();
    }
  }
  for (auto v : value_infoes) {
    if (name_set.count(v->name()) == 0) {
      name_set.insert(v->name());
      SimOnnxCtx::getSimOnnxCtx()->CreateTensorObj(v);
      VLOG(1) << "value_infoes add: " << v->name();
    } else {
      LOG(ERROR) << "duplicated name in value_infoes: " << v->name();
    }
  }

  LOG(INFO) << " === Graph Summary After Read === ";
  LOG(INFO) << "\t Input num:\t" << input_num;
  LOG(INFO) << "\t Output num:\t" << outputs.size();
  LOG(INFO) << "\t Node num:\t" << nodes.size();
  LOG(INFO) << "\t Initializer num:\t" << inits.size();
  LOG(INFO) << "\t ValueInfo num:\t" << value_infoes.size();
  return true;
}

bool SimOnnxCtx::saveOnnx(const std::string path, bool overwrite) {
  LOCK;
  CHECK_BINDED;
  // remove deleted node obj
  if (!applyDeletedObj()) {
    return false;
  }
  return bkctx_->saveFile(path, overwrite);
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
  CHECK_BINDED;
  reset_impl();
}

bool SimOnnxCtx::applyDeletedObj() {
  for (auto& kv : obj_del_ctx_) {
    for (auto& v : kv.second) {
      if (!v->destroyHandle()) {
        return false;
      }
    }
  }
  obj_del_ctx_.clear();
  return true;
}

bool SimOnnxCtx::ctxPass(PassType type) { return bkctx_->pass(type); }

}  // namespace simonnx
}  // namespace utils
