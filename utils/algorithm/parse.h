/**
 * @file parse.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2023  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-01-14 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_ALGORITHM_PARSE_H_
#define UTILS_ALGORITHM_PARSE_H_

#include <glog/logging.h>

#include <charconv>
#include <cstdint>
#include <initializer_list>
#include <regex>  // NOLINT
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "utils/algorithm/strutils.h"

namespace utils {
namespace algorithm {
namespace parse {

class DimVal {
 public:
  explicit DimVal(int64_t val) {
    val_ = val;
    is_val_ = true;
  }
  explicit DimVal(std::string param, bool cast = true) {
    if (cast) {
      try {
        size_t pos;
        std::stoll(param, &pos);
        is_val_ = pos == param.size();
      } catch (const std::invalid_argument&) {
        is_val_ = false;
      } catch (const std::out_of_range&) {
        is_val_ = false;
      }
    } else {
      is_val_ = false;
    }
    if (is_val_) {
      val_ = std::stoll(param);
    } else {
      param_ = param;
    }
  }
  bool isVal() const { return is_val_; }

  operator std::string() const {
    if (is_val_) {
      return std::to_string(val_);
    } else {
      return param_;
    }
  }
  operator int64_t() const {
    if (is_val_) {
      return val_;
    } else {
      return -1;
    }
  }
  DimVal& operator=(int64_t val) {
    is_val_ = true;
    val_ = val;
    return *this;
  }
  DimVal& operator=(std::string param) {
    is_val_ = false;
    param_ = param;
    return *this;
  }

 private:
  bool is_val_ = true;
  int64_t val_ = -1;
  std::string param_ = "";
};

inline std::ostream& operator<<(std::ostream& os, const DimVal& dim_val) {
  return os << static_cast<std::string>(dim_val);
}

#define IS_TYPE(tp) (std::is_same<std::decay<tp>::type, _T>::value)

template <typename _T>
struct Str2Val;

template <>
struct Str2Val<int32_t> {
  static int32_t cast(std::string s) { return std::stoi(s); }
};
template <>
struct Str2Val<int64_t> {
  static int64_t cast(std::string s) { return std::stoll(s); }
};
template <>
struct Str2Val<float> {
  static float cast(std::string s) { return std::stof(s); }
};
template <>
struct Str2Val<double> {
  static double cast(std::string s) { return std::stod(s); }
};
template <>
struct Str2Val<DimVal> {
  static DimVal cast(std::string s) { return DimVal(s); }
};

template <typename _T>
class ArrayStr {
 public:
  using Val_t = _T;
  using Array_t = std::vector<_T>;
  using Str_t = std::string;

 public:
  ArrayStr() : ArrayStr({}) {}
  explicit ArrayStr(std::initializer_list<_T> array)
      : ArrayStr(Array_t(array)) {}
  explicit ArrayStr(Array_t array) : dim_(array) {
    str_ = ArrayStr::Array2Str(dim_);
  }
  explicit ArrayStr(Str_t str) {
    if (StrValid(str)) {
      dim_ = ArrayStr::Str2Array(str);
      str_ = str;
    }
  }

  const bool valid() const { return ArrayStr::StrValid(str_); }
  const Array_t& getArray() const { return dim_; }
  const Str_t& getStr() const { return str_; }

 public:
  static bool StrValid(Str_t str) {
    std::string ele_str;
    if (IS_TYPE(int32_t) || IS_TYPE(int64_t)) {
      ele_str = "-?\\d+";
    } else if (IS_TYPE(float) || IS_TYPE(double)) {
      ele_str = "-?\\d+\\.?\\d*";
    } else if (IS_TYPE(DimVal)) {
      ele_str = "(-?\\d+)|([\\d\\w]+)";
    } else {
      LOG(FATAL) << "error type";
    }
    std::regex pattern(strutils::replace("^\\[ *({ELE}) *(, *({ELE}))*,? *\\]$",
                                         "{ELE}", ele_str));
    return std::regex_match(str, pattern);
  }
  static Array_t Str2Array(Str_t str) {
    Array_t array;
    if (ArrayStr::StrValid(str)) {
      str = strutils::replace(str, "[", "");
      str = strutils::replace(str, "]", "");
      str = strutils::replace(str, " ", "");
      auto token = strutils::split(str, ",");
      for (auto t : token) {
        if (t.size() == 0) {
          continue;
        } else if (IS_TYPE(int32_t)) {
          array.emplace_back(std::stoi(t));
        } else if (IS_TYPE(int64_t)) {
          array.emplace_back(std::stoll(t));
        } else if (IS_TYPE(float)) {
          array.emplace_back(std::stof(t));
        } else if (IS_TYPE(double)) {
          array.emplace_back(std::stod(t));
        } else if (IS_TYPE(DimVal)) {
          array.emplace_back(DimVal(t));
        } else {
          LOG(FATAL) << "error type";
        }
      }
    }
    return array;
  }
  static Str_t Array2Str(Array_t array) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < array.size(); i++) {
      ss << array[i];
      if (i != (array.size() - 1)) {
        ss << ", ";
      }
    }
    ss << "]";
    return ss.str();
  }

 private:
  Array_t dim_;
  Str_t str_;
};

using DimStr = ArrayStr<DimVal>;

}  // namespace parse
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_PARSE_H_
