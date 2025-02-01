import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import Flatcontrols

T.MenuBarItem {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 4
    verticalPadding: 4
    spacing: 6

    icon.width: 16
    icon.height: 16
    icon.color: enabled ? FlatcontrolsStyle.text : FlatcontrolsStyle.toolTipText
    
    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        alignment: Qt.AlignHCenter

        icon: control.icon
        text: control.text
        font: control.font
        color: control.enabled ? FlatcontrolsStyle.text : FlatcontrolsStyle.toolTipText
    }

    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 24
        radius: 2
        color: FlatcontrolsStyle.blendColors(FlatcontrolsStyle.text, FlatcontrolsStyle.base, 80)
        visible: control.highlighted
    }
}
