pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Window
import QtQuick.Controls.impl
import QtQuick.Templates as T
import Flatcontrols

T.HorizontalHeaderView {
    id: control
    implicitWidth: syncView ? syncView.width : 0
    // implicitHeight has to be at least 1 during startup. Otherwise TableView will not load the delegate.
    implicitHeight: Math.max(1, contentHeight)
    delegate: Rectangle {
        id: delegate
        required property var model
        readonly property real cellPadding: 8

        implicitWidth: text.implicitWidth + (cellPadding * 2)
        implicitHeight: Math.max(control.height, text.implicitHeight + (cellPadding * 2))
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