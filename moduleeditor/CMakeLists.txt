find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(moduleeditor STATIC)

qt_add_qml_module(moduleeditor
  URI "ModuleEditor"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET moduleeditorplugin
  VERSION 1.0
  QML_FILES
    qml/ModuleEditor.qml qml/ParamBool.qml qml/ParamEnum.qml qml/ParamNumeric.qml qml/ParamReference.qml qml/ParamString.qml
  SOURCES
    src/moduleeditormodel.cpp src/parameter.cpp
)

set_target_properties(moduleeditor PROPERTIES AUTOMOC ON)

target_include_directories(moduleeditor PUBLIC src)

#target_compile_definitions(moduleeditor
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(moduleeditor PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin
  PUBLIC
  workspacecontroller workspacecontrollerplugin)

