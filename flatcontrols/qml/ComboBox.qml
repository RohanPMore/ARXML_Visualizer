import QtQuick
import QtQuick.Window
import QtQuick.Controls.impl
import QtQuick.Templates as T

import Flatcontrols
import FontAwesome

T.ComboBox {
    FontAwesome {
        id: awesome
    }

    id: control

    property color backgroundColor : FlatcontrolsStyle.base

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    delegate: MenuItem {
        required property int index
        x: 8
        width: backgroundRect.width-16
        text: model[index]
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: Text {
        x: control.mirrored ? control.padding + 6 : control.width - contentWidth - control.padding - 6
        y: control.topPadding + (control.availableHeight - height) / 2
        color: control.editable ? FlatcontrolsStyle.text : FlatcontrolsStyle.buttonText
        font.pointSize: 12
        font.weight: awesome.weight_fa_solid
        font.family: awesome.family
        text: awesome.icons.fas_angle_down
    }

    contentItem: T.TextField {
        leftPadding: 4 - control.padding

        text: control.editable ? control.editText : control.displayText

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator
        selectByMouse: control.selectTextByMouse

        color: control.editable ? FlatcontrolsStyle.text : FlatcontrolsStyle.buttonText
        selectionColor: FlatcontrolsStyle.highlight
        selectedTextColor: FlatcontrolsStyle.highlightedText
        verticalAlignment: Text.AlignVCenter

        cursorDelegate: CursorDelegate { }
    }

    background: TextPanel {
        implicitWidth: 120
        visible: !control.flat || control.down
        highlighted: control.hovered
    }

    popup: T.Popup {
        y: control.height
        width: control.width
        height: Math.min(contentItem.implicitHeight + verticalPadding * 2, control.Window.height - topMargin - bottomMargin)
        transformOrigin: Item.Top
        topMargin: 12
        bottomMargin: 12
        verticalPadding: 8

        enter: Transition {
            NumberAnimation { property: "scale"; from: 0.9; easing.type: Easing.OutQuint; duration: 220 }
            NumberAnimation { property: "opacity"; from: 0.0; easing.type: Easing.OutCubic; duration: 150 }
        }

        exit: Transition {
            NumberAnimation { property: "scale"; to: 0.9; easing.type: Easing.OutQuint; duration: 220 }
            NumberAnimation { property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
        }

        contentItem: ListView {
            width: 50
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0
            T.ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
                id:backgroundRect
                color: Qt.lighter(FlatcontrolsStyle.base, 1.0)
                border.color: FlatcontrolsStyle.outline
                width: parent.width
                height: parent.height
                radius: 3

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
    }
}
