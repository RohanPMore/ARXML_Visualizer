find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(statusbar STATIC)

qt_add_qml_module(statusbar
  URI "Statusbar"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET statusbarplugin
  VERSION 1.0
  QML_FILES
    qml/Statusbar.qml
)

set_target_properties(statusbar PROPERTIES AUTOMOC ON)

target_include_directories(statusbar PUBLIC src)

#target_compile_definitions(statusbar
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(statusbar PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin)


