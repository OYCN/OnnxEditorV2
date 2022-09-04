add_subdirectory(graph)
add_subdirectory(test)

add_library(gui
  gui.cpp
  mainwindow.cpp
  actions.cpp
  res/res.qrc
)

target_link_libraries(gui
  PUBLIC
  Qt6::Widgets
  Qt6::Core
  glog::glog

  graph
  simonnx
  gui_test
)
