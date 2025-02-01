import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import FontAwesome
import Flatcontrols

T.MenuItem {
    FontAwesome {
        id: awesome
    }
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    horizontalPadding: 20
    verticalPadding: 3
    spacing: 6

    icon.width: 10
    icon.height: 10

    contentItem: IconLabel {
        readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.checkable && control.indicator ? control.indicator.width + control.spacing : 0
        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding

        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        alignment: Qt.AlignLeft

        icon: control.icon
        text: control.text
        font: control.font
        color: control.down || control.highlighted ? FlatcontrolsStyle.highlightedText : FlatcontrolsStyle.text
    }

    arrow: Text {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 20

        visible: control.subMenu
        rotation: control.mirrored ? 90 : -90
        font.pointSize: 12
        font.weight: awesome.weight_fa_solid
        font.family: awesome.family
        text: awesome.icons.fas_caret_down
    }

    indicator: CheckIndicator {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        control: control
        visible: control.checkable
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 20

        color: FlatcontrolsStyle.highlight
        visible: control.down || control.highlighted
    }
}
