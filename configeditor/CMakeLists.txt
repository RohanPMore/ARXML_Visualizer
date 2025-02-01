find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(configeditor STATIC)

qt_add_qml_module(configeditor
  URI "ConfigEditor"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET configeditorplugin
  VERSION 1.0
  QML_FILES
    qml/ConfigEditor.qml
    qml/NavigationBar.qml
    qml/NavigationButton.qml
    qml/NavigationHistory.qml
)

set_target_properties(configeditor PROPERTIES AUTOMOC ON)

target_include_directories(configeditor PUBLIC src)

#target_compile_definitions(configeditor
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(configeditor PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin
  moduleeditor moduleeditorplugin
  projecttreeview projecttreeviewplugin
  workspacecontroller workspacecontrollerplugin)

