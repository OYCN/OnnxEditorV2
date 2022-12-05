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

namespace utils {
namespace simonnx {

namespace fs = std::filesystem;

SimOnnxCtx::SimOnnxCtx() {
  mp_ = new ::ONNX_NAMESPACE::ModelProto();
  resetDebugFn();
  resetInfoFn();
  resetErrorFn();
}

bool SimOnnxCtx::openOnnx(const std::string path) {
  this->reset();
  LOG(INFO) << "lock @" << std::this_thread::get_id();
  std::unique_lock lock(mutex_);
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (!fs::exists(path)) {
    errorfn_("Path invalid " + path);
    return false;
  }
  if (fs::is_directory(path)) {
    errorfn_("Path is not a file " + path);
    return false;
  }
  auto omp = this->getModelProtoPtr();
  std::ifstream fin(path, std::ios::in | std::ios::binary);
  if (!fin.good()) {
    errorfn_("Could not open file " + path);
    return false;
  } else {
    bool ret = omp->ParseFromIstream(&fin);
    return ret;
  }
}

bool SimOnnxCtx::saveOnnx(const std::string path, bool overwrite) {
  LOG(INFO) << "lock @" << std::this_thread::get_id();
  std::unique_lock lock(mutex_);
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (fs::exists(path) && !overwrite) {
    errorfn_("Path already exist " + path);
    return false;
  }
  if (fs::is_directory(path)) {
    errorfn_("Path is a directory" + path);
    return false;
  }
  auto omp = this->getModelProtoPtr();
  std::ofstream fout(path, std::ios::out | std::ios::binary);
  if (!fout.good()) {
    errorfn_("Could not open file " + path);
    return false;
  } else {
    bool ret = omp->SerializeToOstream(&fout);
    return ret;
  }
}

void SimOnnxCtx::reset() {
  std::unique_lock lock(mutex_);
  LOG(INFO) << "SimOnnxCtx reset: " << this;
  if (obj_ctx_.size() > 0) {
    for (auto kv : obj_ctx_) {
      for (auto ptr : obj_ctx_.at(kv.first)) {
        delete ptr;
      }
      obj_ctx_.at(kv.first).clear();
    }
  }
}

void SimOnnxCtx::setDebugFn(std::function<void(std::string)> fn) {
  std::unique_lock lock(mutex_);
  debugfn_ = fn;
}
void SimOnnxCtx::setInfoFn(std::function<void(std::string)> fn) {
  std::unique_lock lock(mutex_);
  infofn_ = fn;
}
void SimOnnxCtx::setErrorFn(std::function<void(std::string)> fn) {
  std::unique_lock lock(mutex_);
  errorfn_ = fn;
}
void SimOnnxCtx::resetDebugFn() {
  std::unique_lock lock(mutex_);
  debugfn_ = [](std::string msg) { VLOG(1) << msg; };
}
void SimOnnxCtx::resetInfoFn() {
  std::unique_lock lock(mutex_);
  infofn_ = [](std::string msg) { LOG(INFO) << msg; };
}
void SimOnnxCtx::resetErrorFn() {
  std::unique_lock lock(mutex_);
  errorfn_ = [](std::string msg) { LOG(ERROR) << msg; };
}

}  // namespace simonnx
}  // namespace utils
