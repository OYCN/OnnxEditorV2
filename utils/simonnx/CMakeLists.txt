add_library(simonnx
  onnx_meta.cpp
)

target_link_libraries(simonnx
  PUBLIC
  onnx
  glog::glog
)