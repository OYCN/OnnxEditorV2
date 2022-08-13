add_executable(simonnx_test
  simonnx_test.cpp
)
target_link_libraries(simonnx_test
  GTest::gtest_main
  simonnx
)
gtest_discover_tests(simonnx_test)

add_executable(algorithm_test
algorithm_test.cpp
)
target_link_libraries(algorithm_test
  GTest::gtest_main
  utils_algorithm
)
gtest_discover_tests(algorithm_test)

add_executable(ogdf_proxy
ogdf_proxy.cpp
)
target_link_libraries(ogdf_proxy
  GTest::gtest_main
  external_ogdf
)
gtest_discover_tests(ogdf_proxy)
