# - Try to find Graphviz
# Once done this will define
#
#  GRAPHVIZ_FOUND - system has Graphviz
#  GRAPHVIZ_INCLUDE_DIRS - Graphviz include directories
#  GRAPHVIZ_CDT_LIBRARY - Graphviz CDT library
#  GRAPHVIZ_GVC_LIBRARY - Graphviz GVC library
#  GRAPHVIZ_CGRAPH_LIBRARY - Graphviz CGRAPH library
#  GRAPHVIZ_PATHPLAN_LIBRARY - Graphviz PATHPLAN library
#  GRAPHVIZ_VERSION - Graphviz version
#
# This module reads hints about search locations from the following cmake variables:
#  GRAPHVIZ_ROOT          - Graphviz installation prefix
#                           (containing bin/, include/, etc.)

# Copyright (c) 2009, Adrien Bustany, <madcat@mymadcat.com>
# Copyright (c) 2013-2014 Kevin Funk <kevin.funk@kdab.com>

# Version computation and some cleanups by Allen Winter <allen.winter@kdab.com>
# Copyright (c) 2012-2014 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

# Simplified script by Dogan Can <dogancan@usc.edu>
# Copyright (c) 2014 University of Southern California

# Redistribution and use is allowed according to the terms of the GPLv3+ license.


if(GRAPHVIZ_ROOT)
  set(_GRAPHVIZ_INCLUDE_DIR ${GRAPHVIZ_ROOT}/include)
  set(_GRAPHVIZ_LIBRARY_DIR ${GRAPHVIZ_ROOT}/lib)
endif()

find_path(GRAPHVIZ_INCLUDE_DIR         NAMES graphviz/cgraph.h
          HINTS ${_GRAPHVIZ_INCLUDE_DIR})
find_library(GRAPHVIZ_CDT_LIBRARY      NAMES cdt 
             HINTS ${_GRAPHVIZ_LIBRARY_DIR})
find_library(GRAPHVIZ_GVC_LIBRARY      NAMES gvc 
             HINTS ${_GRAPHVIZ_LIBRARY_DIR})
find_library(GRAPHVIZ_CGRAPH_LIBRARY   NAMES cgraph
             HINTS ${_GRAPHVIZ_LIBRARY_DIR})
find_library(GRAPHVIZ_PATHPLAN_LIBRARY NAMES pathplan
             HINTS ${_GRAPHVIZ_LIBRARY_DIR})
find_library(GRAPHVIZ_GVPLUGIN_CORE_LIBRARY NAMES gvplugin_core
             HINTS ${_GRAPHVIZ_LIBRARY_DIR}/graphviz)
find_library(GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY NAMES gvplugin_dot_layout
             HINTS ${_GRAPHVIZ_LIBRARY_DIR}/graphviz)

if(GRAPHVIZ_INCLUDE_DIR AND GRAPHVIZ_CDT_LIBRARY AND GRAPHVIZ_GVC_LIBRARY
    AND GRAPHVIZ_CGRAPH_LIBRARY AND GRAPHVIZ_PATHPLAN_LIBRARY AND GRAPHVIZ_GVPLUGIN_CORE_LIBRARY AND GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY)
  set(GRAPHVIZ_FOUND TRUE)
else()
  set(GRAPHVIZ_FOUND FALSE)
endif()

# Ok, now compute the version
if(GRAPHVIZ_FOUND)
    set(FIND_GRAPHVIZ_VERSION_SOURCE
      "#include <graphviz/graphviz_version.h>\n#include <stdio.h>\n int main()\n {\n printf(\"%s\",PACKAGE_VERSION);return 1;\n }\n")
    set(FIND_GRAPHVIZ_VERSION_SOURCE_FILE ${CMAKE_BINARY_DIR}/CMakeTmp/FindGRAPHVIZ.cxx)
    file(WRITE "${FIND_GRAPHVIZ_VERSION_SOURCE_FILE}" "${FIND_GRAPHVIZ_VERSION_SOURCE}")
    
    set(FIND_GRAPHVIZ_VERSION_ADD_INCLUDES
      "-DINCLUDE_DIRECTORIES:STRING=${GRAPHVIZ_INCLUDE_DIR}")
    
    try_run(RUN_RESULT COMPILE_RESULT
      ${CMAKE_BINARY_DIR}
      ${FIND_GRAPHVIZ_VERSION_SOURCE_FILE}
      CMAKE_FLAGS "${FIND_GRAPHVIZ_VERSION_ADD_INCLUDES}"
      RUN_OUTPUT_VARIABLE GRAPHVIZ_VERSION)
    
    if(COMPILE_RESULT AND RUN_RESULT EQUAL 1)
      message(STATUS "Graphviz version: ${GRAPHVIZ_VERSION}")
    else()
      message(FATAL_ERROR "Unable to compile or run the graphviz version detection program.")
    endif()
    
    set(GRAPHVIZ_INCLUDE_DIRS ${GRAPHVIZ_INCLUDE_DIR} ${GRAPHVIZ_INCLUDE_DIR}/graphviz)
    
    if(NOT Graphviz_FIND_QUIETLY)
      message(STATUS "Graphviz include: ${GRAPHVIZ_INCLUDE_DIRS}")
      message(STATUS "Graphviz libraries: ${GRAPHVIZ_CDT_LIBRARY} ${GRAPHVIZ_GVC_LIBRARY} ${GRAPHVIZ_CGRAPH_LIBRARY} ${GRAPHVIZ_PATHPLAN_LIBRARY} ${GRAPHVIZ_GVPLUGIN_CORE_LIBRARY} ${GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY}")
    endif()
endif()

if(GRAPHVIZ_FOUND)
  set(GRAPHVIZ_ALL_LIBS ${GRAPHVIZ_CDT_LIBRARY} ${GRAPHVIZ_GVC_LIBRARY} ${GRAPHVIZ_CGRAPH_LIBRARY} ${GRAPHVIZ_PATHPLAN_LIBRARY} ${GRAPHVIZ_GVPLUGIN_CORE_LIBRARY} ${GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY})
  add_library(gvc INTERFACE)
  add_library(cgraph INTERFACE)
  add_library(gvplugin_core INTERFACE)
  add_library(gvplugin_dot_layout INTERFACE)

  target_link_libraries(gvc
    INTERFACE
    ${GRAPHVIZ_ALL_LIBS}
  )
  target_include_directories(gvc
    INTERFACE
    ${GRAPHVIZ_INCLUDE_DIRS}
  )
  
  target_link_libraries(cgraph
    INTERFACE
    ${GRAPHVIZ_ALL_LIBS}
  )
  target_include_directories(cgraph
    INTERFACE
    ${GRAPHVIZ_INCLUDE_DIRS}
  )
  
  target_link_libraries(gvplugin_core
    INTERFACE
    ${GRAPHVIZ_ALL_LIBS}
  )
  
  target_link_libraries(gvplugin_dot_layout
    INTERFACE
    ${GRAPHVIZ_ALL_LIBS}
  )
endif()

if(Graphviz_FIND_REQUIRED AND NOT GRAPHVIZ_FOUND)
  message(FATAL_ERROR "Could not find GraphViz.")
endif()
