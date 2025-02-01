import QtQuick
import Flatcontrols

Item {
    id: itemContainer
    property color background: FlatcontrolsStyle.window
    property color textColor: FlatcontrolsStyle.windowText
    property string text: ""
    signal hovered(rectColor : color)
    signal clicked()

    Rectangle {
        id: backgroundRect
        height: parent.height 
        width: parent.width
        color: background
        Text {
            id: textInRect
            text: qsTr(text)
            anchors.fill: parent
            color: text
        }
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: itemContainer.hovered(itemContainer.background)
        onClicked: itemContainer.clicked()
    }
}