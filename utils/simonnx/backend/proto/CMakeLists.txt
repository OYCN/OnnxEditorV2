add_library(bk_proto STATIC
  bk_ctx.cpp
  bk_graph.cpp
  bk_node.cpp
  bk_attr.cpp
  bk_tensor.cpp
  bk_value_info.cpp
  onnx_pass.cpp
)

target_link_libraries(bk_proto
  PUBLIC
  onnx
  glog::glog
  utils_algorithm
)