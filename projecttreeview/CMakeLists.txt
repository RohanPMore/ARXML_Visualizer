find_package(Qt6 COMPONENTS Gui Quick Core REQUIRED)

qt_add_library(projecttreeview STATIC)

qt_add_qml_module(projecttreeview
  URI "ProjectTreeView"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET projecttreeviewplugin
  VERSION 1.0
  QML_FILES
    qml/ProjectTreeView.qml
  SOURCES
    src/treemodel.cpp src/treeitem.cpp
)

set_target_properties(projecttreeview PROPERTIES AUTOMOC ON)

target_include_directories(projecttreeview PUBLIC src)

#target_compile_definitions(projecttreeview
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(projecttreeview PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin
  workspacecontroller workspacecontrollerplugin)

