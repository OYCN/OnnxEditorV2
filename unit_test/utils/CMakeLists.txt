add_executable(parser_test
parser_test.cpp
)

target_link_libraries(parser_test
  GTest::gtest_main
  onnx_parser
)

gtest_discover_tests(parser_test)
