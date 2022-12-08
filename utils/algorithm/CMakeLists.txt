add_subdirectory(external)

add_library(utils_algorithm STATIC
  topo_sort.cpp
  layout.cpp
)

target_link_libraries(utils_algorithm
  PUBLIC
  
  glog::glog
  magic_enum::magic_enum

  external_ogdf
)
