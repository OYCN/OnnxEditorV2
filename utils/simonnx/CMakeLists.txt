add_library(simonnx
  onnx_help.cpp
)

target_link_libraries(simonnx
  PUBLIC
  onnx
  glog::glog
  magic_enum::magic_enum
  external_ogdf
)