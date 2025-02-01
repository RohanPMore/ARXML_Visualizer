import QtQuick
import Flatcontrols
import FontAwesome

Item {
    id: buttonContainer
    property color background: FlatcontrolsStyle.window
    property color textColor: FlatcontrolsStyle.text
    property string text

    signal clicked()

    Rectangle {
        id: backgroundRect
        height: parent.height
        width: parent.width
        color: mouseArea.containsMouse ? Qt.lighter(background, 1.1) : background
        radius: 2

        Text {
            id: textInRect
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            font.pointSize: parent.height*0.5
            text: buttonContainer.text
            color: buttonContainer.textColor
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: buttonContainer.clicked()
        }
    }
}