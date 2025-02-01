find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(iconbar STATIC)

qt_add_qml_module(iconbar
  URI "Iconbar"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET iconbarplugin
  VERSION 1.0
  QML_FILES
    qml/Iconbar.qml
)

set_target_properties(iconbar PROPERTIES AUTOMOC ON)

target_include_directories(iconbar PUBLIC src)

#target_compile_definitions(iconbar
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(iconbar PRIVATE
  Qt6::Quick Qt6::Gui
  flatcontrols flatcontrolsplugin)

