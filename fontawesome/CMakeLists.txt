find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(fontawesome STATIC)

qt_add_qml_module(fontawesome
  URI "FontAwesome"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET fontawesomeplugin
  VERSION 1.0
  QML_FILES
    qml/Variables.qml
    qml/FontAwesome.qml

)

qt_add_resources(fontawesome "fa-solid-900"
    PREFIX "/"
    FILES resource/fa-solid-900.ttf
)

qt_add_resources(fontawesome "fa-regular-400"
    PREFIX "/"
    FILES resource/fa-regular-400.ttf
)

set_target_properties(fontawesome PROPERTIES AUTOMOC ON)

#target_compile_definitions(fontawesome
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(fontawesome PRIVATE Qt6::Quick Qt6::Gui)

