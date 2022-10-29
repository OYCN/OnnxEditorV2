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

namespace utils {
namespace simonnx {

enum class ObjType_t { kNode = 0, kTensor, kMax };

class IObject {
 public:
  virtual ~IObject() = default;
  virtual size_t getId() const = 0;
  virtual void setId(size_t id) = 0;
  virtual bool isDeleted() const = 0;
  virtual void setDeleted(bool deleted) = 0;
  virtual ObjType_t getObjType() const = 0;
};

template <ObjType_t _OT>
class Object : public IObject {
 public:
  Object() {
    id_ = genNewId();
    getObjList().push_back(this);
    iter_ = getObjList().end();
    --iter_;
  }
  virtual ~Object() { getObjList().erase(iter_); }
  size_t getId() const override { return id_; }
  void setId(size_t id) override { id_ = id; }
  bool isDeleted() const override { return deleted_; }
  void setDeleted(bool deleted) override { deleted_ = deleted; }
  static constexpr ObjType_t ObjType = _OT;
  ObjType_t getObjType() const override { return ObjType; }
  using ObjList_t = std::list<Object<_OT>*>;
  ObjList_t& getObjList() {
    static ObjList_t list;
    return list;
  }
  typename ObjList_t::iterator getObjListIter() { return iter_; }

 private:
  size_t genNewId() {
    static size_t now_idx = 0;
    return now_idx++;
  }

  size_t get() {
    static size_t now_idx = 0;
    return now_idx++;
  }

 private:
  size_t id_;
  typename ObjList_t::iterator iter_;
  bool deleted_ = false;
};

}  // namespace simonnx
}  // namespace utils

#endif  // UTILS_SIMONNX_OBJECT_H_
