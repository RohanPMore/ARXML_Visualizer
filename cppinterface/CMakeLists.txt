find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(cppinterface STATIC)

qt_add_qml_module(cppinterface
  URI "CppInterface"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET cppinterfaceplugin
  VERSION 1.0
  SOURCES
    src/cppinterface.cpp src/cppinterface.h
    src/bswmdfindertask.cpp src/bswmdfindertask.h
    src/dpa_file.cpp src/dpa_file.h
    src/ecucparsertask.cpp src/ecucparsertask.h
    src/bswmodule.cpp src/bswmodule.h
    src/projectinfo.h
    src/referenceresolvertask.cpp src/referenceresolvertask.h
    src/projectloadertask.cpp src/projectloadertask.h
)

set_target_properties(cppinterface PROPERTIES AUTOMOC ON)

target_include_directories(cppinterface PUBLIC src)

#target_compile_definitions(cppinterface
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(cppinterface
  PRIVATE Qt6::Quick Qt6::Gui
  rapidxml
  arxml_parser
  ecuc_parser
  bswmd_parser)

