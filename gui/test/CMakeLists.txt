
add_library(gui_test
  random_nodes.cpp
)

target_link_libraries(gui_test
  PUBLIC
  Qt6::Widgets
  glog::glog
)
