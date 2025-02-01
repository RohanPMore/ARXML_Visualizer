import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

import Flatcontrols

T.TextField {
    id: control

    property color backgroundColor : FlatcontrolsStyle.base

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset
                   || Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    padding: 4

    color: FlatcontrolsStyle.text
    selectionColor: FlatcontrolsStyle.highlight
    selectedTextColor: FlatcontrolsStyle.highlightedText
    placeholderTextColor: FlatcontrolsStyle.placeholderText
    verticalAlignment: TextInput.AlignVCenter
    
    cursorDelegate: CursorDelegate { }

    PlaceholderText {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: control.placeholderTextColor
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
        renderType: control.renderType
    }

    background: TextPanel {
        implicitWidth: 120
        implicitHeight: 24
        highlighted: control.hovered
    }
}
