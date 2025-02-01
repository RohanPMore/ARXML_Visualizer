import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import Flatcontrols

T.Button {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 4
    //spacing: 6

    icon.width: 16
    icon.height: 16

    icon.color: !enabled ? FlatcontrolsStyle.placeholderText :
        (control.flat && control.highlighted) || (control.checked && !control.highlighted) ? FlatcontrolsStyle.highlight :
        highlighted ? FlatcontrolsStyle.highlight : FlatcontrolsStyle.buttonText

    readonly property bool hasIcon: icon.name.length > 0 || icon.source.toString().length > 0

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.checked || control.highlighted ? FlatcontrolsStyle.brightText :
               control.flat && !control.down ? (control.visualFocus ? FlatcontrolsStyle.highlight : FlatcontrolsStyle.windowText) : FlatcontrolsStyle.buttonText
    }

    background: Item {
        Rectangle {
            implicitWidth: 64
            implicitHeight: 24
            width: parent.width
            height: parent.height

            radius: 3
            color: control.highlighted ? Qt.lighter(FlatcontrolsStyle.button, 1.04) : control.down ? Qt.darker(FlatcontrolsStyle.button, 1.1) : FlatcontrolsStyle.button
            border.color: control.hovered ? FlatcontrolsStyle.highlightedOutline : FlatcontrolsStyle.outline
            border.width: 1
        }
    }
}
