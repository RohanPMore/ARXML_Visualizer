find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_library(flatcontrols STATIC)

qt_add_qml_module(flatcontrols
  URI "Flatcontrols"
  RESOURCE_PREFIX "AsrConfModules/"
  PLUGIN_TARGET flatcontrolsplugin
  VERSION 1.0
  QML_FILES
    qml/Button.qml
    qml/ComboBox.qml
    qml/Label.qml
    qml/Menu.qml
    qml/MenuBar.qml
    qml/MenuBarItem.qml
    qml/MenuItem.qml
    qml/CursorDelegate.qml
    qml/HorizontalHeaderView.qml
    qml/ScrollIndicator.qml
    qml/CheckIndicator.qml
    qml/Popup.qml
    qml/ScrollBar.qml
    qml/SplitView.qml
    qml/TabBar.qml
    qml/TabButton.qml
    qml/TextArea.qml
    qml/TextPanel.qml
    qml/TextField.qml
    qml/ToolTip.qml
    qml/CheckBox.qml
    qml/VerticalHeaderView.qml
  SOURCES
    src/flatcontrolsstyle.cpp src/flatcontrolsstyle.h
)

set_target_properties(flatcontrols PROPERTIES AUTOMOC ON)

target_include_directories(flatcontrols PUBLIC src)

#target_compile_definitions(flatcontrols
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(flatcontrols
  PRIVATE Qt6::Quick Qt6::Gui
  fontawesome fontawesomeplugin)

