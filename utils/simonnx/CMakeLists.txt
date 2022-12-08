add_library(simonnx STATIC
  onnx_help.cpp
  context.cpp
  node.cpp
  tensor.cpp
)

target_link_libraries(simonnx
  PUBLIC
  onnx
  glog::glog
  magic_enum::magic_enum
  external_ogdf
)