import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Qt.labs.qmlmodels

import Flatcontrols
import ModuleEditor
import WorkspaceController

Item {
    id: moduleEditorFrame
    required property WorkspaceController workspace
    required property int projectId
    signal showReferencedItem(string path)
    anchors.leftMargin: 20
    anchors.rightMargin: 20

    function loadContainer(path:string)
    {
        paramList.model.loadContainer(path)
    }

    Rectangle {
        anchors.fill: parent
        color: FlatcontrolsStyle.alternateBase
    }
    
    ListView {
        id: paramList
        anchors.fill: parent
        clip: true
        spacing: 5
        ScrollBar.vertical: ScrollBar { }
        property int parameterNameWidth
        property int defaultLineHeight
        property int defaultFontPixels

        parameterNameWidth: 300
        defaultFontPixels: defaultLineHeight * 0.5
        defaultLineHeight: 25

        model: ModuleEditorModel { 
            projectId: moduleEditorFrame.projectId
            workspace: moduleEditorFrame.workspace
        }

        DelegateChooser {
            id: chooser
            role: "type"

            DelegateChoice { roleValue: ParameterType.BOOL; ParamBool {} }
            DelegateChoice { roleValue: ParameterType.REFERENCE; ParamReference {} }
            DelegateChoice { roleValue: ParameterType.ENUMERATION; ParamEnum {} }
            DelegateChoice { roleValue: ParameterType.INTEGER; ParamNumeric {} }
            DelegateChoice { roleValue: ParameterType.FLOAT; ParamNumeric {} }
            DelegateChoice { roleValue: ParameterType.STRING; ParamString {} }
            DelegateChoice { roleValue: ParameterType.LINKER_SYMBOL; ParamString {} }
            DelegateChoice { roleValue: ParameterType.FUNCTION_NAME; ParamString {} }
        }

        delegate: chooser
    }
}
