/**
 * @file graph_desc.h
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

#ifndef UTILS_ALGORITHM_GRAPH_DESC_H_
#define UTILS_ALGORITHM_GRAPH_DESC_H_

#include <map>
#include <string>
#include <vector>

namespace utils {
namespace algorithm {
namespace desc {

// Note: edge and node need be normed to 0 ~ n

class GraphNode2EdgeDesc {
 public:
  /**
   * @brief Get node length
   * @return size_t
   */
  virtual size_t getLen() const = 0;
  /**
   * @brief Get output of node
   * @param  i                Node id
   * @return std::vector<size_t> Edge id
   */
  virtual std::vector<size_t> getOutput(size_t i) const = 0;
  /**
   * @brief Get input of node
   * @param  i                Node id
   * @return std::vector<size_t> Edge id
   */
  virtual std::vector<size_t> getInput(size_t i) const = 0;
};

class GraphNode2NodeDesc {
 public:
  /**
   * @brief Get node length
   * @return size_t
   */
  virtual size_t getLen() const = 0;
  /**
   * @brief Get output of node
   * @param  i                Node id
   * @return std::vector<size_t> Node id
   */
  virtual std::vector<size_t> getOutput(size_t i) const = 0;
  /**
   * @brief Get input of node
   * @param  i                Node id
   * @return std::vector<size_t> Node id
   */
  virtual std::vector<size_t> getIntput(size_t i) const = 0;
  /**
   * @brief Get the root nodes
   * @return std::vector<size_t>
   */
  virtual std::vector<size_t> getRoot() const = 0;

  virtual std::pair<float, float> getWH(size_t i) const = 0;
};

class GraphNode2NodeDescTmp : virtual public GraphNode2NodeDesc {
 public:
  GraphNode2NodeDescTmp(size_t len, std::vector<std::vector<size_t>> outputs,
                        std::vector<std::vector<size_t>> inputs,
                        std::vector<std::pair<float, float>> whs,
                        std::vector<size_t> roots)
      : len_(len),
        outputs_(outputs),
        inputs_(inputs),
        whs_(whs),
        roots_(roots) {}

 public:
  size_t getLen() const override { return len_; }
  std::vector<size_t> getOutput(size_t i) const override { return outputs_[i]; }
  std::vector<size_t> getIntput(size_t i) const override { return inputs_[i]; }
  std::vector<size_t> getRoot() const override { return roots_; }
  std::pair<float, float> getWH(size_t i) const override { return whs_[i]; }

 public:
  size_t len_;
  std::vector<std::vector<size_t>> outputs_;
  std::vector<std::vector<size_t>> inputs_;
  std::vector<std::pair<float, float>> whs_;
  std::vector<size_t> roots_;
};

class GraphNode2NodeDescExt : virtual public GraphNode2NodeDesc {
 public:
  virtual std::vector<std::string> getNodeName(size_t i) const = 0;
  virtual std::vector<std::pair<std::string, std::string>> getNodeAttrs(
      size_t i) const = 0;
  virtual std::string getEdgeName(size_t i, size_t j) const = 0;
};

class GraphNode2NodeDescExtTmp : public GraphNode2NodeDescExt,
                                 public GraphNode2NodeDescTmp {
 public:
  GraphNode2NodeDescExtTmp(
      GraphNode2NodeDescTmp g, std::vector<std::vector<std::string>> node2name,
      std::vector<std::vector<std::pair<std::string, std::string>>> node2attrs,
      std::map<std::pair<size_t, size_t>, std::string> edge2name)
      : GraphNode2NodeDescTmp(g),
        node2name_(node2name),
        node2attrs_(node2attrs),
        edge2name_(edge2name) {}

 public:
  std::vector<std::string> getNodeName(size_t i) const override { return node2name_[i]; }
  std::vector<std::pair<std::string, std::string>> getNodeAttrs(
      size_t i) const override {
    return node2attrs_[i];
  }
  std::string getEdgeName(size_t i, size_t j) const override {
    return edge2name_.at(std::pair<size_t, size_t>(i, j));
  }

 public:
  std::vector<std::vector<std::string>> node2name_;
  std::vector<std::vector<std::pair<std::string, std::string>>> node2attrs_;
  std::map<std::pair<size_t, size_t>, std::string> edge2name_;
};

class GraphNode2LayoutDesc {
 public:
  struct Pos_t {
    float x;
    float y;
  };

 public:
  virtual size_t getLen() const = 0;
  virtual Pos_t getNodePos(size_t i) const = 0;
};

class GraphNode2LayoutDescTmp : public GraphNode2LayoutDesc {
 public:
  GraphNode2LayoutDescTmp(size_t len, std::vector<Pos_t> node_pos)
      : len_(len), node_pos_(node_pos) {}

 public:
  size_t getLen() const override { return len_; }
  Pos_t getNodePos(size_t i) const override { return node_pos_[i]; }

 private:
  size_t len_;
  std::vector<Pos_t> node_pos_;
};

}  // namespace desc
}  // namespace algorithm
}  // namespace utils

#endif  // UTILS_ALGORITHM_GRAPH_DESC_H_
