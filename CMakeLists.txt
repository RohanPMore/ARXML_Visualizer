cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0071 NEW)

project(casimir LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (WIN64)
  set(CMAKE_PREFIX_PATH "C:\\Qt\\6.6.1\\mingw_64\\lib\\cmake")
ELSE()
  # set stuff for other systems
ENDIF()


find_package(Qt6 COMPONENTS Gui Quick REQUIRED)

qt_add_executable(casimir main.cpp)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_property(TARGET casimir PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(casimir PUBLIC .)

#add_subdirectory(argparse)
SET(USE_INTERNAL_RAPIDXML OFF CACHE BOOL "Use common rapidxml library for arxml-parser" FORCE)
add_subdirectory(arxml_parser)

add_subdirectory(rapidxml-1.13)
add_subdirectory(configeditor)
add_subdirectory(containertable)
add_subdirectory(bswmd-parser)
add_subdirectory(ecuc-parser)
add_subdirectory(moduleeditor)
add_subdirectory(projecttreeview)
add_subdirectory(fontawesome)
add_subdirectory(statusbar)
add_subdirectory(iconbar)
add_subdirectory(flatcontrols)
#add_subdirectory(ecuprojectmodel)
add_subdirectory(workspacecontroller)
add_subdirectory(cppinterface)


#target_compile_definitions(casimir
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(casimir PRIVATE
  Qt6::Quick Qt6::Gui
  arxml_parser
  bswmd_parser
  ecuc_parser
  moduleeditor moduleeditorplugin
  projecttreeview projecttreeviewplugin
  fontawesome fontawesomeplugin
  statusbar statusbarplugin
  iconbar iconbarplugin
  configeditor configeditorplugin
  workspacecontroller workspacecontrollerplugin
  cppinterface cppinterfaceplugin
  containertable containertableplugin)

qt_add_qml_module(casimir
  URI config_main
  VERSION 1.0
  QML_FILES main.qml)
