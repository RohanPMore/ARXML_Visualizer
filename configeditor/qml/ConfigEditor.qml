import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Qt.labs.qmlmodels

import Flatcontrols
import ModuleEditor
import ProjectTreeView
import ConfigEditor
import WorkspaceController
import ContainerTable

Item {
    id: configEditorRoot
    required property WorkspaceController workspace
    required property int projectId
    property NavigationHistory navHistory: NavigationHistory { }
    focus: true
    Keys.onPressed: (event)=> {
        if(event.key == Qt.Key_Left) {
            navHistory.moveBack()
            event.accepted = true;
        } else if(event.key == Qt.Key_Right) {
            navHistory.moveForward()
            event.accepted = true;
        }
    }
    signal addressChanged(ecucPath:string)

    onAddressChanged: (ecucPath) => {
        if(ecucPath.includes(":"))
        {
            showTableView(ecucPath);
        }
        else
        {
            showModuleEditor(ecucPath);
        }
        navBar.addressChanged(ecucPath);
    }

    function showModuleEditor(ecucPath: string) {
        moduleEditor.loadContainer(ecucPath);
        stackLayout.currentIndex = 0;
    }

    function showTableView(ecucPath: string) {
        containerTable.loadContainers(ecucPath);
        stackLayout.currentIndex = 1;
    }


    MouseArea {
        anchors.fill: parent
        acceptedButtons: (Qt.BackButton | Qt.ForwardButton)
        hoverEnabled: false
        onClicked: (mouse) => {
            mouse.accepted = false
            if (mouse.button == Qt.BackButton) {
                navHistory.moveBack();
                mouse.accepted = true;
            } else if (mouse.button == Qt.ForwardButton) {
                navHistory.moveForward();
                mouse.accepted = true;
            } else {
                mouse.accepted = false
            }
        }

        NavigationBar {
            id: navBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 35
            onForwardClicked: navHistory.moveForward()
            onBackClicked: navHistory.moveBack()
            
        }

        SplitView {
            id: splitView
            anchors.top: navBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            ProjectTreeView {
                id:treeView
                workspace: configEditorRoot.workspace
                projectId: configEditorRoot.projectId
                SplitView.minimumWidth: 100
                SplitView.preferredWidth: 250
                SplitView.minimumHeight: 150
                SplitView.preferredHeight: 200
            }
            // SplitView seems to ignore margins.
            // This dummy item is used to provide margins to its contents
            Rectangle
            {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 500
                SplitView.preferredWidth: 1000
                SplitView.minimumHeight: 150
                SplitView.preferredHeight: 100
                color: FlatcontrolsStyle.alternateBase
                StackLayout
                {
                    anchors.fill: parent
                    anchors.margins: 10
                    id:stackLayout
                    ModuleEditor {
                        id: moduleEditor
                        workspace: configEditorRoot.workspace
                        projectId: configEditorRoot.projectId
                    }
                    ContainerTable {
                        id: containerTable
                        workspace: configEditorRoot.workspace
                        projectId: configEditorRoot.projectId
                    }
                }

                Component.onCompleted: {
                    // User clicked a reference in the module editor
                    moduleEditor.showReferencedItem.connect(navHistory.push)

                    // User selected a container in the tree view
                    treeView.containerSelected.connect(navHistory.push)

                    // User selected a container in the table view
                    containerTable.containerSelected.connect(navHistory.push)

                    // User selected a container type in the tree view
                    treeView.containerTypeSelected.connect(navHistory.push)

                    // User clicked a link in the navigation bar
                    navBar.linkActivated.connect(navHistory.push)
                    
                    // Upon navigation change, update the treeview, stacklayout (table or moduleeditor) and navbar:
                    navHistory.moveTo.connect(configEditorRoot.addressChanged)
                    navHistory.moveTo.connect(treeView.selectContainer)
                }
            }
        }
    }
}