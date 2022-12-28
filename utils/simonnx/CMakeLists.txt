add_library(simonnx STATIC
  context.cpp
  node.cpp
  tensor.cpp
  onnx_pass.cpp
)

target_link_libraries(simonnx
  PUBLIC
  onnx
  glog::glog
  magic_enum::magic_enum
  external_ogdf
  utils_algorithm
)