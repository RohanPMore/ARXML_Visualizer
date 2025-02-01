add_library(bswmd_parser STATIC)

target_sources(bswmd_parser PRIVATE
  bswmd_parser.cc
  module_def.cc
  param_container_def.cc
  bool_def.cc
  enum_def.cc
  enum_literal_def.cc
  int_def.cc
  function_name_def.cc
  float_def.cc
  reference_def.cc
  choice_reference_def.cc
  choice_container_def.cc
  string_def.cc
  symbolic_name_reference_def.cc)

target_include_directories(bswmd_parser PUBLIC .)

target_link_libraries(bswmd_parser PUBLIC arxml_parser)

