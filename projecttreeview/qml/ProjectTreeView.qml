import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import ProjectTreeView
import FontAwesome
import Flatcontrols
import WorkspaceController

Frame {
    id: treeViewFrame
    required property WorkspaceController workspace 
    required property int projectId 
    signal containerSelected(ecucPath:string)
    signal containerTypeSelected(ecucBswmdPath:string)
    signal selectContainer(ecucPath:string)

    FontAwesome {
        id: awesome
    }
    background: Rectangle {
        color: FlatcontrolsStyle.alternateBase
    }
    onSelectContainer: ecucPath => {
        var mi = treeView.model.indexFromEcucPath(ecucPath)
        if(mi.valid)
        {
            if(!treeView.selectionModel.selectedIndexes.includes(mi))
            {
                treeView.selectionModel.select(mi, ItemSelectionModel.Select | ItemSelectionModel.Current)
                treeView.expandToIndex(mi)
                treeView.forceLayout()
                treeView.positionViewAtRow(treeView.rowAtIndex(mi), Qt.AlignVCenter)
            }
        }
    }

    TreeView {
        id: treeView
        anchors.fill: parent
        model: TreeModel { 
            workspace: treeViewFrame.workspace 
            projectId: treeViewFrame.projectId
        }
        selectionModel: ItemSelectionModel { }
        clip: true
        selectionBehavior: TableView.SelectRows
        ScrollBar.vertical: ScrollBar { }
        delegate: MouseArea {
            id: treeDelegate
            acceptedButtons: (Qt.LeftButton | Qt.RightButton)
            propagateComposedEvents: true

            implicitWidth: treeView.width
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indent: 20
            readonly property real padding: 5

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth
            required property int row
            required property bool selected
            required property bool current

            Rectangle {
                id: background
                anchors.fill: parent
                color: treeDelegate.selected
                    ? FlatcontrolsStyle.highlight
                    : (treeView.alternatingRows && treeDelegate.row % 2 !== 0
                    ? FlatcontrolsStyle.alternateBase
                    : FlatcontrolsStyle.base)
            }

            TapHandler {
                onTapped: 
                {
                    treeView.toggleExpanded(row)
                }
            }

            Text {
                id: indicator
                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                x: treeDelegate.padding + (treeDelegate.depth * treeDelegate.indent)
                anchors.verticalCenter: label.verticalCenter
                font.pointSize: 13
                font.weight: awesome.weight_fa_solid
                font.family: awesome.family
                text: awesome.icons.fas_angle_right
                color: FlatcontrolsStyle.text
                rotation: treeDelegate.expanded ? 90 : 0
            }

            Text {
                x: treeDelegate.padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
                anchors.verticalCenter: label.verticalCenter
                font.pointSize: 13
                font.weight: awesome.weight_fa_solid
                font.family: awesome.family
                color: FlatcontrolsStyle.text
                text: switch(type) {
                        case TreeItemType.MODULE: return awesome.icons.fas_layer_group
                        case TreeItemType.CONTAINERTYPE: return awesome.icons.fas_folder
                        case TreeItemType.CONTAINER: return awesome.icons.fas_sliders
                      }
            }

            Text {
                id: label
                x: treeDelegate.padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0) + 30
                anchors.verticalCenter: background.verticalCenter
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                color: FlatcontrolsStyle.text
                font.pixelSize: 12
                text: model.title
            }

            onClicked: (mouse) => {
                if(mouse.button == Qt.LeftButton) {
                    mouse.accepted = true
                    var mi=treeView.index(row, column)
                    treeView.selectionModel.select(mi, ItemSelectionModel.Select | ItemSelectionModel.Current)
                    if(type == TreeItemType.CONTAINERTYPE) {
                        // Show the tableview for all instances of the container:
                        containerTypeSelected(ecucPath+":"+model.title)
                    } else {
                        // Show the selected container
                        containerSelected(ecucPath)
                    }
                    
                } else {
                    mouse.accepted = false
                }
            }
        }
    }
}
