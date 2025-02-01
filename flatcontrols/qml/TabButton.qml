import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import Flatcontrols

T.TabButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    width: implicitWidth

    padding: 6
    spacing: 6

    icon.width: 24
    icon.height: 24
    icon.color: !enabled ? FlatcontrolsStyle.toolTipText : down || checked ? FlatcontrolsStyle.text : FlatcontrolsStyle.toolTipText
    

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: !control.enabled ? FlatcontrolsStyle.toolTipText : control.down || control.checked ? FlatcontrolsStyle.text : FlatcontrolsStyle.toolTipText
    }

    background: Rectangle {
        implicitHeight: 30
        implicitWidth: 70
        
        color: control.checked ? FlatcontrolsStyle.alternateBase : FlatcontrolsStyle.base
        Rectangle {
            height: parent.height
            width: 1
            color: FlatcontrolsStyle.button
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Rectangle {
            height: control.checked ? 0 : 1
            width: parent.width
            color: FlatcontrolsStyle.button
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }
    }
}
