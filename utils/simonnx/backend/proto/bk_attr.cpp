/**
 * @file bk_attr.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-11 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/simonnx/backend/proto/bk_attr.h"

#include <glog/logging.h>
#include <onnx/onnx_pb.h>

#include "utils/simonnx/backend/proto/bk_node.h"
#include "utils/simonnx/backend/proto/helper.h"

#define CHECK_IS_DT(dt)                           \
  (ONNX_NAMESPACE::AttributeProto_AttributeType:: \
       AttributeProto_AttributeType_##dt == handle_->type())

#define CHECK_HAS_DT(dt, op) (CHECK_IS_DT(dt) && handle_->has_##op())

#define SET_TYPE(dt)                                              \
  setType(handle_, ONNX_NAMESPACE::AttributeProto_AttributeType:: \
                       AttributeProto_AttributeType_##dt)

#define CLEAR_DT(dt, op)                              \
  case ONNX_NAMESPACE::AttributeProto_AttributeType:: \
      AttributeProto_AttributeType_##dt:              \
    handle->clear_##op();                             \
    return true;

#define CLEAR_DT_S(dt, op)                            \
  case ONNX_NAMESPACE::AttributeProto_AttributeType:: \
      AttributeProto_AttributeType_##dt:              \
    handle->mutable_##op()->Clear();                  \
    return true;

#define DEF_METHORD_ELE(dt, op, type, def)       \
  type ProtoBackendAttribute::op() const {       \
    if (CHECK_HAS_DT(dt, op)) {                  \
      return handle_->op();                      \
    } else {                                     \
      return def;                                \
    }                                            \
  }                                              \
  bool ProtoBackendAttribute::set_##op(type v) { \
    if (SET_TYPE(dt)) {                          \
      CHECK(CHECK_IS_DT(dt));                    \
      handle_->set_##op(v);                      \
      return true;                               \
    } else {                                     \
      return false;                              \
    }                                            \
  }

#define DEF_METHORD_ELES(dt, op, type)            \
  type ProtoBackendAttribute::op() const {        \
    type ret;                                     \
    if (CHECK_IS_DT(dt)) {                        \
      auto eles = handle_->mutable_##op();        \
      for (size_t i = 0; i < eles->size(); i++) { \
        ret.emplace_back(eles->Get(i));           \
      }                                           \
    }                                             \
    return ret;                                   \
  }                                               \
  bool ProtoBackendAttribute::set_##op(type vs) { \
    if (SET_TYPE(dt)) {                           \
      CHECK(CHECK_IS_DT(dt));                     \
      auto eles = handle_->mutable_##op();        \
      eles->Clear();                              \
      for (auto v : vs) {                         \
        *eles->Add() = v;                         \
      }                                           \
      return true;                                \
    } else {                                      \
      return false;                               \
    }                                             \
  }

namespace utils {
namespace simonnx {
namespace backend {
namespace proto {

bool clear_by_dt(AttributeProtoPtr handle,
                 ONNX_NAMESPACE::AttributeProto_AttributeType dt) {
  if (handle == nullptr) {
    return false;
  }
  switch (dt) {
    CLEAR_DT(FLOAT, f)
    CLEAR_DT(INT, i)
    CLEAR_DT(STRING, s)
    CLEAR_DT(TENSOR, t)
    CLEAR_DT(GRAPH, g)
    CLEAR_DT_S(FLOATS, floats)
    CLEAR_DT_S(INTS, ints)
    CLEAR_DT_S(TENSORS, tensors)
    CLEAR_DT_S(GRAPHS, graphs)
    case ONNX_NAMESPACE::AttributeProto_AttributeType_UNDEFINED:
      return true;
    default:
      LOG(ERROR) << "will clear a unhandled dt: "
                 << ONNX_NAMESPACE::AttributeProto_AttributeType_Name(dt);
      return false;
  }
}

bool setType(AttributeProtoPtr handle,
             ONNX_NAMESPACE::AttributeProto_AttributeType dt) {
  if (handle == nullptr) {
    return false;
  }
  if (!ONNX_NAMESPACE::AttributeProto_AttributeType_IsValid(dt)) {
    return false;
  }
  auto old_dt = handle->type();
  if (old_dt != dt) {
    if (!clear_by_dt(handle, old_dt)) {
      return false;
    }
  }
  handle->set_type(dt);
  return true;
}

ProtoBackendAttribute::ProtoBackendAttribute(ProtoBackendNode* parent,
                                             AttributeProtoPtr handle)
    : parent_(parent), handle_(handle) {}

bool ProtoBackendAttribute::destroy() {
  VLOG(1) << "delete NodeProtoPtr";
  auto attrs = parent_->getHandle()->mutable_attribute();
  if (delFromRepeatProto(attrs, handle_)) {
    handle_ = nullptr;
    LOG(INFO) << "delete Attr success";
    return true;
  } else {
    LOG(ERROR) << "delete Attr failed @" << handle_;
    return false;
  }
}

std::string ProtoBackendAttribute::name() const {
  CHECK_HANDLE_DEL("");
  return handle_->name();
}

bool ProtoBackendAttribute::set_name(const std::string& name) {
  CHECK_HANDLE_DEL(false);
  handle_->set_name(name);
  return true;
}
std::string ProtoBackendAttribute::type() const {
  CHECK_HANDLE_DEL("");
  auto data_type = handle_->type();
  CHECK(ONNX_NAMESPACE::AttributeProto_AttributeType_IsValid(data_type));
  CHECK(ONNX_NAMESPACE::AttributeProto_AttributeType_IsValid(data_type));
  auto t = static_cast<ONNX_NAMESPACE::AttributeProto_AttributeType>(data_type);
  if (t == ONNX_NAMESPACE::AttributeProto_AttributeType_UNDEFINED) {
    if (handle_->has_i()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_INT;
    } else if (handle_->has_f()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_FLOAT;
    } else if (handle_->has_s()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_STRING;
    } else if (handle_->has_t()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_TENSOR;
    } else if (handle_->has_g()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_GRAPH;
    } else if (handle_->ints_size()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_INTS;
    } else if (handle_->floats_size()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_FLOATS;
    } else if (handle_->strings_size()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_STRINGS;
    } else if (handle_->tensors_size()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_TENSORS;
    } else if (handle_->graphs_size()) {
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_GRAPHS;
    }
    if (!setType(handle_, t)) {
      LOG(ERROR) << "attr type fallback error";
      t = ONNX_NAMESPACE::AttributeProto_AttributeType_UNDEFINED;
    }
  }
  return ONNX_NAMESPACE::AttributeProto_AttributeType_Name(t);
}

DEF_METHORD_ELE(INT, i, int, 0);
DEF_METHORD_ELE(FLOAT, f, float, 0.0);
DEF_METHORD_ELE(STRING, s, std::string, "");

DEF_METHORD_ELES(INTS, ints, std::vector<int>);
DEF_METHORD_ELES(FLOATS, floats, std::vector<float>);

}  // namespace proto
}  // namespace backend
}  // namespace simonnx
}  // namespace utils
