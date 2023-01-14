/**
 * @file strutils.h
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

#ifndef UTILS_ALGORITHM_STRUTILS_H_
#define UTILS_ALGORITHM_STRUTILS_H_

#include <string>
#include <vector>

namespace utils {
namespace algorithm {
namespace strutils {

inline std::vector<std::string> split(const std::string& s,
                                      const std::string& delimiter) {
  if (delimiter.empty()) {
    return {s};
  }
  size_t pos_start = 0;
  size_t pos_end = 0;
  size_t delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;
  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }
  res.push_back(s.substr(pos_start));
  return res;
}

inline std::vector<std::string> split(const std::string& str,
                                      const char pattern) {
  std::string delimiter(&pattern, &pattern + 1);
  return split(str, delimiter);
}

inline std::string replace(const std::string& _str, const std::string& before,
                           const std::string& after) {
  std::string str(_str);
  for (std::string::size_type pos(0); pos != std::string::npos;
       pos += after.length()) {
    pos = str.find(before, pos);
    if (pos != std::string::npos) {
      str.replace(pos, before.length(), after);
    } else {
      break;
    }
  }
  return str;
}

}  // namespace strutils
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_STRUTILS_H_
