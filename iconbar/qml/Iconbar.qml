import QtQuick
import QtQuick.Layouts
import FontAwesome
import Flatcontrols

Item {
    FontAwesome {
        id: awesome
    }
    id: itemContainer
    property color background: FlatcontrolsStyle.window
    property color textColor: FlatcontrolsStyle.windowText
    property string text: ""
    property int selectedIndex: 0
    signal hovered(rectColor : color)
    signal clicked()

    Rectangle {
            width: 1
            color: FlatcontrolsStyle.button
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: backgroundRect.right
            }
    }

    Rectangle{
        id: backgroundRect
        width: parent.width - 1
        height: parent.height
        color: background
        Text {
            id: textInRect
            text: qsTr(text)
            anchors.fill: parent
            color: text
        }
    }

    ColumnLayout {
        id: topIcons
        spacing: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10

        Text {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.leftMargin: 10
            font.pointSize: 22
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            text: awesome.icons.fas_sliders
            color: FlatcontrolsStyle.text
        }

        Text {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.leftMargin: 10
            font.pointSize: 22
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            text: awesome.icons.fas_magnifying_glass
            color: Qt.darker(FlatcontrolsStyle.text, 1.3)
        }

        Text {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.leftMargin: 10
            font.pointSize: 22
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            text: awesome.icons.fas_code_branch
            color: Qt.darker(FlatcontrolsStyle.text, 1.3)
        }
    }
    
    ColumnLayout {
        id: bottomIcons
        spacing: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 10

        Text {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.leftMargin: 10
            font.pointSize: 22
            font.weight: awesome.weight_fa_solid
            font.family: awesome.family
            text: awesome.icons.fas_gear
            color: FlatcontrolsStyle.text
        }
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: itemContainer.hovered(itemContainer.background)
        onClicked: itemContainer.clicked()
    }
}