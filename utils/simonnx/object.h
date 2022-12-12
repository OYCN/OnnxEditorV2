/**
 * @file object.h
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

#ifndef UTILS_SIMONNX_OBJECT_H_
#define UTILS_SIMONNX_OBJECT_H_

#include <cstddef>
#include <list>
#include <map>

namespace utils {
namespace simonnx {

class SimOnnxCtx;

enum class ObjType_t { kNode = 0, kTensor, kMax };

class IObject {
 public:
  virtual ~IObject() = default;
  virtual size_t getId() const = 0;
  virtual bool isDeleted() const = 0;
  virtual void setDeleted(bool deleted) = 0;
  virtual ObjType_t getObjType() const = 0;
  virtual SimOnnxCtx* getCtx() const = 0;
  virtual bool hasAttr(std::string key) const = 0;
  virtual std::string getAttr(std::string key) const = 0;
  virtual void setAttr(std::string key, std::string value) = 0;

 protected:
  virtual void setId(size_t id) = 0;
};

template <ObjType_t _OT>
class Object : public IObject {
 public:
  size_t getId() const override { return id_; }
  bool isDeleted() const override { return deleted_; }
  void setDeleted(bool deleted) override { deleted_ = deleted; }
  static constexpr ObjType_t ObjType = _OT;
  ObjType_t getObjType() const override { return ObjType; }
  SimOnnxCtx* getCtx() const override { return ctx_; }
  bool hasAttr(std::string key) const override { return attrs.count(key) != 0; }
  std::string getAttr(std::string key) const override { return attrs.at(key); }
  void setAttr(std::string key, std::string value) override {
    attrs[key] = value;
  }

 protected:
  explicit Object(SimOnnxCtx* ctx) : ctx_(ctx) {}

 private:
  friend class SimOnnxCtx;
  void setId(size_t id) override { id_ = id; }

 private:
  SimOnnxCtx* ctx_ = nullptr;
  size_t id_;
  bool deleted_ = false;
  std::map<std::string, std::string> attrs;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_OBJECT_H_
