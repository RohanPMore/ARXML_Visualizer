find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(workspacecontroller STATIC)

qt_add_qml_module(workspacecontroller
  URI "WorkspaceController"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET workspacecontrollerplugin
  VERSION 1.0
  SOURCES
    src/workspacecontroller.cpp src/workspacecontroller.h
)

set_target_properties(workspacecontroller PROPERTIES AUTOMOC ON)

target_include_directories(workspacecontroller PUBLIC src inc)

target_sources(workspacecontroller PRIVATE
  src/ecuprojectmodel.cpp
  src/ecuprojectmodel.h
  )

#target_compile_definitions(workspacecontroller
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(workspacecontroller PRIVATE
  Qt6::Quick Qt6::Gui
  rapidxml
  PUBLIC
  bswmd_parser
  ecuc_parser
  arxml_parser
  cppinterface
  )

