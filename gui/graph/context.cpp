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
 * <tr><td>2022-12-24 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "gui/graph/context.h"

namespace gui {
namespace graph {

Context::Context(QObject* parent) : QObject(parent) {}

Context::~Context() = default;

void Context::nodeUpdateSend(Node* node) { emit nodeUpdateSignal(node); }

}  // namespace graph
}  // namespace gui
