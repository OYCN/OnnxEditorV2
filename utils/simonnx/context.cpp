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

namespace utils {
namespace simonnx {

namespace fs = std::filesystem;

SimOnnxCtx::SimOnnxCtx() { mp_ = new ::ONNX_NAMESPACE::ModelProto(); }

bool SimOnnxCtx::openOnnx(const std::string path) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (!fs::exists(path)) {
    LOG(ERROR) << "Path invalid " << path;
    return false;
  }
  if (fs::is_directory(path)) {
    LOG(ERROR) << "Path is not a file " << path;
    return false;
  }
  this->reset();
  auto omp = this->getModelProtoPtr();
  std::ifstream fin(path, std::ios::in | std::ios::binary);
  if (!fin.good()) {
    LOG(ERROR) << "Could not open file " << path;
    return false;
  } else {
    bool ret = omp->ParseFromIstream(&fin);
    return ret;
  }
}

void SimOnnxCtx::reset() {
  if (obj_ctx_.size() > 0) {
    for (auto kv : obj_ctx_) {
      for (auto ptr : obj_ctx_.at(kv.first)) {
        delete ptr;
      }
      obj_ctx_.at(kv.first).clear();
    }
  }
}

}  // namespace simonnx
}  // namespace utils
