
add_subdirectory(graph)

add_library(gui
  gui.cpp
  mainwindow.cpp
)

target_link_libraries(gui
  PUBLIC
  Qt6::Widgets
  Qt6::Core

  graph
)
