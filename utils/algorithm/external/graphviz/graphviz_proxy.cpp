/**
 * @file graphviz_proxy.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-12-30 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/algorithm/external/graphviz/graphviz_proxy.h"

#include <cgraph.h>
#include <glog/logging.h>
#include <gvc.h>

#include <vector>

#define GRAPHVIZ_F 72.0

extern "C" {
extern gvplugin_library_t gvplugin_core_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
}

namespace utils {
namespace algorithm {
namespace external {
namespace graphviz {

void agattr_proxy(Agraph_t* g, int kind, std::string k, const std::string& v) {
  agattr(g, kind, k.data(), v.c_str());
}

void agsafeset_proxy(void* g, std::string k, const std::string& v,
                     const std::string& d) {
  agsafeset(g, k.data(), v.c_str(), d.c_str());
}

Agnode_t* agnode_proxy(Agraph_t* g, std::string name, int createflag) {
  return agnode(g, name.data(), createflag);
}

Agedge_t* agedge_proxy(Agraph_t* g, Agnode_t* t, Agnode_t* h, std::string name,
                       int createflag) {
  return agedge(g, t, h, name.data(), createflag);
}

void initAttr(Agraph_t* g) {
  // graph attr
  agattr_proxy(g, AGRAPH, "rankdir", "TB");
  agattr_proxy(g, AGRAPH, "dpi", std::to_string(GRAPHVIZ_F));
  agattr_proxy(g, AGRAPH, "nodesep", std::to_string(30 / GRAPHVIZ_F));
  agattr_proxy(g, AGRAPH, "ranksep", std::to_string(70 / GRAPHVIZ_F));
  agattr_proxy(g, AGRAPH, "concentrate", "true");
  // node attr
  agattr_proxy(g, AGNODE, "fixedsize", "true");
  agattr_proxy(g, AGNODE, "shape", "box");
  // edge attr
  // agattr(g, AGEDGE, "", "");
}

// GraphNode2NodeDescTmp cvt(const Agraph_t* g) {

// }

void cvt(const GraphNode2NodeDesc* gd, Agraph_t* g) {
  CHECK_NOTNULL(g);
  initAttr(g);
  std::vector<Agnode_t*> nodes(gd->getLen());
  for (size_t i = 0; i < nodes.size(); i++) {
    auto name = std::to_string(i);
    nodes[i] = agnode_proxy(g, name.c_str(), true);
    CHECK_NOTNULL(nodes[i]);
    auto wh = gd->getWH(i);
    auto w_str = std::to_string(wh.first / GRAPHVIZ_F);
    auto h_str = std::to_string(wh.second / GRAPHVIZ_F);
    VLOG(1) << "node: " << i;
    VLOG(1) << "set wh: " << w_str << ", " << h_str;
    agsafeset_proxy(nodes[i], "width", w_str.c_str(), "1");
    agsafeset_proxy(nodes[i], "height", h_str.c_str(), "1");
  }
  for (size_t i = 0; i < gd->getLen(); i++) {
    auto u = nodes[i];
    auto vids = gd->getOutput(i);
    for (auto vid : vids) {
      auto v = nodes[vid];
      CHECK_NOTNULL(u);
      CHECK_NOTNULL(v);
      auto name = std::to_string(i) + " to " + std::to_string(vid);
      auto e = agedge_proxy(g, u, v, name.c_str(), true);
      CHECK_NOTNULL(e);
    }
  }
}

std::pair<GVC_t*, Agraph_t*> gd2layout(const GraphNode2NodeDesc* gd) {
  GVC_t* gvc = gvContext();
  CHECK_NOTNULL(gvplugin_core_LTX_library.packagename);
  CHECK_NOTNULL(gvplugin_dot_layout_LTX_library.packagename);
  gvAddLibrary(gvc, &gvplugin_core_LTX_library);
  gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library);
  Agraph_t* g = agopen(nullptr, Agdirected, nullptr);
  cvt(gd, g);
  if (gvLayout(gvc, g, "dot") != 0) {
    LOG(FATAL) << "GraphViz dot layout failed";
  }
  return {gvc, g};
}

void freeAll(GVC_t* gvc, Agraph_t* g) {
  gvFreeLayout(gvc, g);
  agclose(g);
  gvFreeContext(gvc);
}

void layout2file(const GraphNode2NodeDesc* gd, const std::string& fmt,
                 std::string path) {
  auto ret = gd2layout(gd);
  GVC_t* gvc = ret.first;
  Agraph_t* g = ret.second;
  gvRenderFilename(gvc, g, fmt.c_str(), path.data());
  freeAll(gvc, g);
}

void layout2dot(const GraphNode2NodeDesc* gd, std::string path) {
  layout2file(gd, "dot", path);
  LOG(INFO) << "vis on http://magjac.com/graphviz-visual-editor/";
}
void layout2svg(const GraphNode2NodeDesc* gd, std::string path) {
  layout2file(gd, "svg", path);
}

GraphNode2LayoutDescTmp layout(const GraphNode2NodeDesc* gd) {
  std::vector<GraphNode2LayoutDescTmp::Pos_t> node_pos(gd->getLen(), {0, 0});
  auto ret = gd2layout(gd);
  GVC_t* gvc = ret.first;
  Agraph_t* g = ret.second;
  for (size_t i = 0; i < node_pos.size(); i++) {
    auto name = std::to_string(i);
    auto n = agnode_proxy(g, name.c_str(), false);
    CHECK_NOTNULL(n);
    auto pos = ND_coord(n);
    node_pos[i].x = - pos.x;
    node_pos[i].y = - pos.y;
    VLOG(1) << "node: " << i;
    VLOG(1) << "wh: " << ND_width(n) << ", " << ND_height(n);
    VLOG(1) << "coord: " << node_pos[i].x << ", " << node_pos[i].y;
  }
  freeAll(gvc, g);
  return GraphNode2LayoutDescTmp(node_pos.size(), node_pos);
}

}  // namespace graphviz
}  // namespace external
}  // namespace algorithm
}  // namespace utils
