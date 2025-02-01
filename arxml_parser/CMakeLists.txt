add_library(arxml_parser STATIC)

find_package(Boost 1.80 REQUIRED)

message(Boost_INCLUDE_DIRS="${Boost_INCLUDE_DIRS}")
target_include_directories(arxml_parser PUBLIC ${Boost_INCLUDE_DIRS})

option(USE_INTERNAL_RAPIDXML "Use rapidxml implementation included in arxml-parser" ON)

message(USE_INTERNAL_RAPIDXML="${USE_INTERNAL_RAPIDXML}")
if (USE_INTERNAL_RAPIDXML)
    add_subdirectory(rapidxml-1.13)
endif()

target_link_libraries(arxml_parser rapidxml)

target_include_directories(arxml_parser PUBLIC inc)

target_sources(arxml_parser PRIVATE src/referrable.cc src/arpackage.cc src/armodel.cc src/arxml_file.cc)

