add_subdirectory(proto)

add_library(backend STATIC
  backend.cpp
)

target_link_libraries(backend
  PUBLIC
  onnx
  glog::glog
  bk_proto
)