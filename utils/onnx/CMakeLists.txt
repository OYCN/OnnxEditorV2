add_library(onnx_parser
  parser.cpp
)

target_link_libraries(onnx_parser
  PRIVATE
  onnx
)