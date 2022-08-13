
add_library(graph
  view.cpp
  scene.cpp
  node.cpp
)

target_link_libraries(graph
  PUBLIC
  Qt6::Widgets
  Qt6::Core
  glog::glog
)
