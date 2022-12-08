add_library(external_ogdf STATIC
 ogdf_proxy.cpp
)

target_link_libraries(external_ogdf
  PUBLIC
  OGDF
  glog::glog
)
