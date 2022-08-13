
add_subdirectory(nlohmann-json EXCLUDE_FROM_ALL)

set(BUILD_GMOCK OFF)
add_subdirectory(gtest EXCLUDE_FROM_ALL)
add_subdirectory(gflags EXCLUDE_FROM_ALL)
# set(WITH_GFLAGS OFF)
# set(WITH_GTEST OFF)
add_subdirectory(glog EXCLUDE_FROM_ALL)

add_subdirectory(magic_enum EXCLUDE_FROM_ALL)

set(protobuf_BUILD_SHARED_LIBS OFF)
set(protobuf_BUILD_TESTS OFF)
add_subdirectory(protobuf EXCLUDE_FROM_ALL)

set(ONNX_ML ON)
set(ONNX_GEN_PB_TYPE_STUBS OFF)
add_definitions(-DONNX_ML=1)
add_definitions(-DONNX_NAMESPACE=onnx)
add_subdirectory(onnx EXCLUDE_FROM_ALL)

add_subdirectory(ogdf EXCLUDE_FROM_ALL)
