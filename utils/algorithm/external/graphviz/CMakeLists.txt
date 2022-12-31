add_library(external_graphviz STATIC
 graphviz_proxy.cpp
)

target_link_libraries(external_graphviz
  PUBLIC
  gvc
  cgraph
  gvplugin_core
  gvplugin_dot_layout
  glog::glog
)
