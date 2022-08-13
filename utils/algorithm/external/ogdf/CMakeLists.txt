add_library(external_ogdf
ogdf_proxy.cpp
)

target_link_libraries(external_ogdf
  PUBLIC
  OGDF
  glog::glog
)
