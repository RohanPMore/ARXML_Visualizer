import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Flatcontrols
import FontAwesome

Item {
    FontAwesome {
        id: awesome
    }
    
    height: view.implicitHeight
    width: paramList.width

    RowLayout {
        anchors.fill: parent
        Label {
            Layout.minimumWidth: paramList.parameterNameWidth
            Layout.maximumWidth: paramList.parameterNameWidth
            Layout.alignment: Qt.AlignTop
            font.pixelSize: paramList.defaultFontPixels
            text: model.name + ":"
        }
        ScrollView {
            id: view
            Layout.fillWidth: true
            Layout.minimumWidth: 20
            Layout.preferredWidth: 100
            Layout.alignment: Qt.AlignTop
            implicitHeight: model.upperMultiplicity > 1 ? (paramList.defaultFontPixels + 12) * 5 : (paramList.defaultFontPixels + 12)

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TextArea {
                id:addressField
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.bottomMargin: 5

                font.pixelSize: paramList.defaultFontPixels
                color: FlatcontrolsStyle.text
                text: "<style>a:link{color: " + FlatcontrolsStyle.link + ";text-decoration: none;}</style>"+model.value
                selectionColor: FlatcontrolsStyle.highlight
                readOnly: true
                textFormat: TextEdit.RichText
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.PointingHandCursor
                }
                onLinkActivated: (ecucPath) => {
                    moduleEditorFrame.showReferencedItem(ecucPath);
                }
            }
        }
        Button {
            id: selectButton
            Layout.fillHeight: true
            Layout.minimumWidth: 40
            Layout.maximumWidth: 40
            topPadding: 0
            bottomPadding: 0
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            text: awesome.icons.fas_pen_to_square
            visible: model.upperMultiplicity == 1
        }
    }
}
