pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T
import Flatcontrols

T.VerticalHeaderView {
    id: control

    // implicitWidth has to be at least 1 during startup. Otherwise TableView will not load the delegate.
    implicitWidth: Math.max(1, contentWidth)
    implicitHeight: syncView ? syncView.height : 0

    delegate: Rectangle {
        id: delegate

        required property var model
        readonly property real cellPadding: 8

        implicitWidth: Math.max(control.width, text.implicitWidth + (cellPadding * 2))
        implicitHeight: text.implicitHeight + (cellPadding * 2)
        color: FlatcontrolsStyle.base

        Label {
            id: text
            text: delegate.model[control.textRole]
            font.bold: true
            width: delegate.width
            height: delegate.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: enabled ? FlatcontrolsStyle.text : FlatcontrolsStyle.placeHolderText
        }

                // Double line separator
        Rectangle {
            id:bottomSeparator
            height: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: FlatcontrolsStyle.outline
        }
        Rectangle {
            height: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bottomSeparator.top
            anchors.bottomMargin: 2
            color: FlatcontrolsStyle.outline
        }
    }
}
