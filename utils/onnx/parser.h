/**
 * @file parser.h
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#ifndef UTILS_ONNX_PARSER_H_
#define UTILS_ONNX_PARSER_H_

#include "utils/onnx/onnx.h"

namespace utils {
namespace onnx {

class Parser {
 public:
  Parser();
  ~Parser() {}

 public:
  SimOnnx parse(void* data, std::size_t size);
};

}  // namespace onnx
}  // namespace utils

#endif  // UTILS_ONNX_PARSER_H_
