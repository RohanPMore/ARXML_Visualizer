import QtQuick
import QtQuick.Templates as T
import QtQuick.Window

import Flatcontrols

T.Menu {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    margins: 0
    padding: 1
    overlap: 2

    transformOrigin: !cascade ? Item.Top : (mirrored ? Item.TopRight : Item.TopLeft)

    delegate: MenuItem { }

    enter: Transition {
        NumberAnimation { property: "scale"; from: 0.9; to: 1.0; easing.type: Easing.OutQuint; duration: 150 }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    exit: Transition {
        NumberAnimation { property: "scale"; from: 1.0; to: 0.9; easing.type: Easing.OutQuint; duration: 150 }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    contentItem: ListView {
        implicitHeight: contentHeight
        model: control.contentModel
        interactive: Window.window
                        ? contentHeight + control.topPadding + control.bottomPadding > Window.window.height
                        : false
        clip: true
        currentIndex: control.currentIndex

        ScrollIndicator.vertical: ScrollIndicator {}
    }

    background: Rectangle {
        id: backgroundRect
        implicitWidth: 200
        implicitHeight: 20
        radius: 3
        color: FlatcontrolsStyle.window
        border.color: FlatcontrolsStyle.outline
        border.width: 1
        Rectangle {
            z: -1
            x: 1; y: 1
            width: parent.width
            height: parent.height
            color: FlatcontrolsStyle.shadow
            opacity: 0.5
            radius: parent.radius
        }
    }

    T.Overlay.modal: Rectangle {
        color: FlatcontrolsStyle.base
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    T.Overlay.modeless: Rectangle {
        color: FlatcontrolsStyle.base
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }
}
