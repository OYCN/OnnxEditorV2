/**
 * @file ogdf_proxy.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-28 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include "utils/algorithm/external/ogdf/ogdf_proxy.h"

#include <glog/logging.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>

#include <map>

namespace utils {
namespace algorithm {
namespace external {
namespace ogdf {

void SugiyamaLayout(::ogdf::GraphAttributes& GA) {
  ::ogdf::SugiyamaLayout SL;
  SL.setRanking(new ::ogdf::OptimalRanking);
  SL.setCrossMin(new ::ogdf::MedianHeuristic);

  ::ogdf::OptimalHierarchyLayout* ohl = new ::ogdf::OptimalHierarchyLayout;
  ohl->layerDistance(30.0);
  ohl->nodeDistance(25.0);
  ohl->weightBalancing(0.8);
  SL.setLayout(ohl);

  SL.call(GA);
}

GraphNode2NodeDescTmp cvt(const ::ogdf::Graph G,
                          std::pair<float, float> hw = {0, 0}) {
  ::ogdf::List<::ogdf::node> nodes;
  G.allNodes(nodes);
  size_t len = G.numberOfNodes();
  CHECK_EQ(len, nodes.size());
  std::map<int, size_t> id2node;
  size_t i = 0;
  for (::ogdf::node node : nodes) {
    CHECK_EQ(id2node.count(node->index()), 0);
    id2node[node->index()] = i++;
  }
  std::vector<std::vector<size_t>> outputs(len);
  std::vector<std::vector<size_t>> inputs(len);
  std::vector<size_t> roots;
  for (::ogdf::node node : nodes) {
    ::ogdf::List<::ogdf::edge> in_edges;
    ::ogdf::List<::ogdf::edge> out_edges;
    CHECK_EQ(id2node.count(node->index()), 1);
    auto id = id2node.at(node->index());
    node->inEdges(in_edges);
    node->outEdges(out_edges);
    if (in_edges.size() == 0) {
      roots.push_back(id);
      VLOG(1) << "[ogdf::G->]add root " << id;
    } else {
      for (::ogdf::edge edge : in_edges) {
        auto opp_node = edge->opposite(node);
        CHECK_EQ(id2node.count(opp_node->index()), 1);
        inputs[id].push_back(id2node.at(opp_node->index()));
        VLOG(1) << "[ogdf::G->]add input map " << id << " to "
                << id2node.at(opp_node->index());
      }
    }
    for (::ogdf::edge edge : out_edges) {
      auto opp_node = edge->opposite(node);
      CHECK_EQ(id2node.count(opp_node->index()), 1);
      outputs[id].push_back(id2node.at(opp_node->index()));
      VLOG(1) << "[ogdf::G->]add output map " << id << " to "
              << id2node.at(opp_node->index());
    }
  }
  std::vector<std::pair<float, float>> whs(len, hw);
  return GraphNode2NodeDescTmp(len, outputs, inputs, whs, roots);
}

std::pair<::ogdf::Graph, ::ogdf::GraphAttributes> cvt(
    const GraphNode2NodeDesc* g) {
  std::pair<::ogdf::Graph, ::ogdf::GraphAttributes> G_GA;
  ::ogdf::Graph& G = G_GA.first;
  G_GA.second =
      ::ogdf::GraphAttributes(G, ::ogdf::GraphAttributes::nodeGraphics |
                                     ::ogdf::GraphAttributes::edgeGraphics |
                                     ::ogdf::GraphAttributes::nodeLabel |
                                     ::ogdf::GraphAttributes::edgeStyle |
                                     ::ogdf::GraphAttributes::nodeStyle |
                                     ::ogdf::GraphAttributes::edgeArrow |
                                     ::ogdf::GraphAttributes::nodeTemplate);
  ::ogdf::GraphAttributes& GA = G_GA.second;
  std::vector<::ogdf::node> nodes(g->getLen());
  for (size_t i = 0; i < nodes.size(); i++) {
    nodes[i] = G.newNode(i);
    VLOG(1) << "[->ogdf::G]add node " << i;
    auto wh = g->getWH(i);
    GA.width(nodes[i]) = wh.first;
    GA.height(nodes[i]) = wh.second;
    VLOG(1) << "[->ogdf::G]set wh to " << wh.first << ", " << wh.second;
  }
  for (size_t i = 0; i < nodes.size(); i++) {
    for (size_t ti : g->getOutput(i)) {
      auto e = G.newEdge(nodes[i], nodes[ti]);
      VLOG(1) << "[->ogdf::G]add edge from " << i << " to " << ti;
    }
  }
  return G_GA;
}

GraphNode2NodeDescTmp genRandomGraph(size_t n, size_t m) {
  ::ogdf::Graph G;
  randomSimpleGraph(G, n, m);
  return cvt(G, {10, 10});
}

void toGml(const GraphNode2NodeDesc* g, std::string path, bool layout) {
  auto G_GA = cvt(g);
  if (layout) {
    SugiyamaLayout(G_GA.second);
  }
  ::ogdf::GraphIO::write(G_GA.first, path, ::ogdf::GraphIO::writeGML);
}

void toSvg(const GraphNode2NodeDesc* g, std::string path, bool layout) {
  auto G_GA = cvt(g);

  if (layout) {
    SugiyamaLayout(G_GA.second);
  }
  ::ogdf::GraphIO::write(G_GA.second, path, ::ogdf::GraphIO::drawSVG);
}

GraphNode2LayoutDescTmp layout(const GraphNode2NodeDesc* g) {
  auto G_GA = cvt(g);
  auto& G = G_GA.first;
  auto& GA = G_GA.second;
  SugiyamaLayout(GA);

  std::vector<GraphNode2LayoutDescTmp::Pos_t> node_pos(g->getLen());
  ::ogdf::List<::ogdf::node> nodes;
  G.allNodes(nodes);
  size_t len = G.numberOfNodes();
  CHECK_EQ(len, nodes.size());
  CHECK_EQ(len, node_pos.size());
  for (::ogdf::node node : nodes) {
    auto id = node->index();
    CHECK_LT(id, node_pos.size());
    CHECK_GE(id, 0);
    node_pos[id].x = GA.x(node);
    node_pos[id].y = GA.y(node);
    VLOG(1) << "node " << id << " pos( " << GA.x(node) << ", " << GA.y(node)
            << ")";
  }

  return GraphNode2LayoutDescTmp(node_pos.size(), node_pos);
}

}  // namespace ogdf
}  // namespace external
}  // namespace algorithm
}  // namespace utils
