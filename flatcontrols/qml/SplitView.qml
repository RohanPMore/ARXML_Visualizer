import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import Flatcontrols

T.SplitView {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    handle: Rectangle {
        id: handleDelegate
        color: parent.T.SplitHandle.pressed ? "transparent": parent.T.SplitHandle.hovered ? "transparent" : FlatcontrolsStyle.button
        implicitWidth: control.orientation === Qt.Horizontal ? 1 : control.width
        implicitHeight: control.orientation === Qt.Horizontal ? control.height : 1
        containmentMask: Item {
            x: control.orientation === Qt.Horizontal ? 0: (handleDelegate.width - width) / 2
            y: control.orientation === Qt.Horizontal ? (handleDelegate.height - height) / 2 : 0 
            width: control.orientation === Qt.Horizontal ? 6 : control.width
            height: control.orientation === Qt.Horizontal ? control.height : 6
        }
    }
}
