find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(containertable STATIC)

qt_add_qml_module(containertable
  URI "ContainerTable"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET containertableplugin
  VERSION 1.0
  QML_FILES
    qml/ContainerTable.qml
  SOURCES
    src/containertablemodel.cpp
)

set_target_properties(containertable PROPERTIES AUTOMOC ON)

target_include_directories(containertable PUBLIC src)

#target_compile_definitions(moduleeditor
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(containertable PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin
  PUBLIC
  workspacecontroller workspacecontrollerplugin)

