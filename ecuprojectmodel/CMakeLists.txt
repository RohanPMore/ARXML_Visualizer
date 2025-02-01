find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(ecuprojectmodel STATIC)

qt_add_qml_module(ecuprojectmodel
  URI "EcuProjectModel"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET ecuprojectmodelplugin
  VERSION 1.0
  QML_FILES
  SOURCES
    src/ecuprojectmodel.cpp src/ecuprojectmodel.h
)

set_target_properties(ecuprojectmodel PROPERTIES AUTOMOC ON)

target_include_directories(ecuprojectmodel PUBLIC src)

target_sources(ecuprojectmodel PRIVATE
  src/ecuprojectmodel.cpp
  src/bswmodule.cpp)

#target_compile_definitions(ecuprojectmodel
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(ecuprojectmodel
  PRIVATE
  Qt6::Quick Qt6::Gui
  PUBLIC
  bswmd_parser
  ecuc_parser
  arxml_parser)

