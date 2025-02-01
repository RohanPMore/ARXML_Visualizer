import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import ConfigEditor
import Statusbar
import Iconbar
import WorkspaceController
import CppInterface

import Flatcontrols
import FontAwesome

ApplicationWindow {
    id:appWindow
    visible: true
    width: 1872
    height: 984
    title: qsTr("casimir")

    FontAwesome {
        id: awesome
    }

    Connections {
        target: CppInterface
        function onEcucFolderOpened(path) { 
            console.log ("Folder: " + path)
            workspaceController.open_ecuc_folder(path)
        }
        function onEcucFileOpened(path) { 
            console.log ("File: " + path)
            workspaceController.open_ecuc_file(path)
        }
        function onProjectLoaded(projectInfo) {
            workspaceController.projectParsed(projectInfo)
        }
    }
    Rectangle {
        anchors.fill: parent
        color: FlatcontrolsStyle.base
        DropArea {
            id: dropArea;
            anchors.fill: parent
            onDropped: function(drop) {
                if(drop.urls.length > 0) {
                    if(drop.urls[0].toString().startsWith("file://") &&
                       drop.urls[0].toString().toLowerCase().endsWith(".dpa")) {
                        CppInterface.openDpaFile(drop.urls[0]);
                    }
                }
            }

            Text {
                id: dropIndicator
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 60
                font.weight: awesome.weight_fa_solid
                font.family: awesome.family
                text: awesome.icons.fas_file_arrow_down
                color: FlatcontrolsStyle.blendColors(FlatcontrolsStyle.text, FlatcontrolsStyle.base, 60)
            }

        }
    }
    

    WorkspaceController {
        id:workspaceController
        onProjectLoaded: (id) => {
            createEditorTab(id) 
        }
    }

    function createEditorTab(id: int) {
        var editor = configEditor.createObject(stackLayout, {workspace: workspaceController, projectId: id});
        tabbar.addItem(tabButton.createObject(tabbar, {text: workspaceController.projectName(id)}))
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    Iconbar {
        id:iconBar
        width: 50
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: statusbar.top
    }

    Component
    {
        id:tabButton
        TabButton {
            text: name
            width: 100

            // MouseArea disabled because reordering the tabs causes the indices of the tabbar and the stacklayout to get out of sync!
            // TODO: Implement proper model for the tabbar that allows reordering while still providing the correct indices to StackLayout. Then activate the following lines again.

            //Drag.active: mouseArea.drag.active
            //Drag.hotSpot.x: width/2
            //Drag.hotSpot.y: height/2
            
            // MouseArea {
            //     id: mouseArea

            //     anchors.fill: parent
            //     property bool held: false

            //     drag.target: parent
            //     drag.axis: Drag.XAxis

            //     onReleased: parent.Drag.drop()
            //     onPressed: { 
            //         tabbar.setCurrentIndex(parent.TabBar.index);
            //     }
            // }

            // states: [
            //     State {
            //         when: mouseArea.drag.active
            //         name: "dragging"
            //         PropertyChanges {
            //             target: parent
            //             opacity: 0.9
            //             anchors.fill: undefined
            //         }
            //     }
            // ]

            DropArea {
                id: dropLeft
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2
                onDropped: function(drop) {
                    drop.source.TabBar.tabBar.moveItem(drop.source.TabBar.index, parent.TabBar.index)
                    drop.source.y = 0
                }
            }

            DropArea {
                id: dropRight
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2
                onDropped: function(drop) {
                    drop.source.TabBar.tabBar.moveItem(drop.source.TabBar.index, parent.TabBar.index+1);
                    drop.source.y = 0;
                }
            }

            Rectangle {
                id: dropHighlightRight
                height: parent.height
                width: dropRight.containsDrag ? 3 : 0
                color: palette.highlight
                anchors.horizontalCenter: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: dropHighlightLeft
                height: parent.height
                width: dropLeft.containsDrag ? 3 : 0
                color: palette.highlight
                anchors.horizontalCenter: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
        }
    }
    TabBar {
        id: tabbar
        height: 30
        anchors.left: iconBar.right
        anchors.right: parent.right
        anchors.top: parent.top

        DropArea {
            id: freeSpaceDrop
            anchors.fill: parent
        }
    }
        
    Component {
        id: configEditor
        ConfigEditor { }
    }

    StackLayout {
        id: stackLayout
        anchors.left: iconBar.right
        anchors.right: parent.right
        anchors.top: tabbar.bottom
        anchors.bottom: statusbar.top
        currentIndex: tabbar.currentIndex
    }

    Statusbar {
        id:statusbar
        height: 20
        width: parent.width
        anchors.bottom: parent.bottom
    }


}
