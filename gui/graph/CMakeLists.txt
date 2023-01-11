
add_library(graph STATIC
  view.cpp
  scene.cpp
  node.cpp
  edge.cpp
  scene_menu.cpp
  context.cpp
)

target_link_libraries(graph
  PUBLIC
  Qt6::Widgets
  Qt6::Core
  glog::glog

  utils_algorithm
)
