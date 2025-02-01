import QtQuick
import QtQuick.Templates as T

import Flatcontrols

T.TabBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    spacing: 0

    contentItem: ListView {
        model: control.contentModel
        currentIndex: control.currentIndex

        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapOneItem

        highlightMoveDuration: 100
        highlightResizeDuration: 0
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: 48
        preferredHighlightEnd: width - 48

        highlight: Item {
            z: 2
            Rectangle {
                height: 2
                width: parent.width
                y: control.position === T.TabBar.Footer ? parent.height - height : 0  
                color: FlatcontrolsStyle.highlight
            }
        }
    }

    background: Rectangle {
        implicitHeight: 21
        color: FlatcontrolsStyle.window
        Rectangle {
            id: bottomSep
            y: control.position === T.TabBar.Header ? parent.height - 1 : 0
            width: parent.width
            height: 1
            color: FlatcontrolsStyle.button
        }
    }
}
