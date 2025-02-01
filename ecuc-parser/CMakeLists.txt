add_library(ecuc_parser STATIC)

target_sources(ecuc_parser PRIVATE 
  ecuc_parser.cc
  module_cfg.cc
  num_param_val.cc
  text_param_val.cc
  param_container_val.cc
  reference_val.cc )

target_include_directories(ecuc_parser PUBLIC .)

target_link_libraries(ecuc_parser PRIVATE arxml_parser)

