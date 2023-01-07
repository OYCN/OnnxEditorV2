/**
 * @file bk_ctx.cpp
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

#include "utils/simonnx/backend/proto/bk_ctx.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include <filesystem>
#include <fstream>

#include "utils/simonnx/backend/proto/bk_graph.h"
#include "utils/simonnx/backend/proto/onnx_pass.h"

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

namespace fs = std::filesystem;

ProtoBackendCtx::ProtoBackendCtx() { mp_ = new ::ONNX_NAMESPACE::ModelProto(); }
ProtoBackendCtx::~ProtoBackendCtx() { delete mp_; }

bool ProtoBackendCtx::loadFile(const std::string& path) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (!fs::exists(path)) {
    LOG(ERROR) << "Path invalid " << path;
    return false;
  }
  if (fs::is_directory(path)) {
    LOG(ERROR) << "Path is not a file " << path;
    return false;
  }
  std::ifstream fin(path, std::ios::in | std::ios::binary);
  if (!fin.good()) {
    LOG(ERROR) << "Could not open file " << path;
    return false;
  }
  if (!mp_->ParseFromIstream(&fin)) {
    LOG(ERROR) << "proto parse error";
    return false;
  }
  mp_->graph();
  graph_ = std::make_shared<ProtoBackendGraph>(this, mp_->mutable_graph());
  return true;
}

bool ProtoBackendCtx::saveFile(const std::string& path, bool overwrite) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (!pass(PassType::kAll)) {
    return false;
  }
  if (fs::exists(path) && !overwrite) {
    LOG(ERROR) << "Path already exist " << path;
    return false;
  }
  if (fs::is_directory(path)) {
    LOG(ERROR) << "Path is a directory " << path;
    return false;
  }
  std::ofstream fout(path, std::ios::out | std::ios::binary);
  if (!fout.good()) {
    LOG(ERROR) << "Could not open file " << path;
    return false;
  } else {
    bool ret = mp_->SerializeToOstream(&fout);
    return ret;
  }
}

SBackendGraph ProtoBackendCtx::graph() const { return graph_; }

bool ProtoBackendCtx::pass(PassType type) {
  if (type == PassType::kNone) {
    return true;
  }
  switch (type) {
    case PassType::kNone:
      return true;
      break;
    case PassType::kAll:
      return OnnxPass::pass_all(mp_);
      break;
    case PassType::kTopoSort:
      return OnnxPass::node_topo_sort(mp_);
      break;
    default:
      LOG(ERROR) << "unsupport pass type";
      return false;
      break;
  }
}

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
