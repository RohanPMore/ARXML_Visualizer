import QtQuick

QtObject {
    readonly property bool canUndo: __canMoveBack()
    readonly property bool canRedo: __canMoveForward()

    readonly property bool clean: __isClean()

    property var __commands: []
    property int __command_index: -1
    property int __clean_index: -1

    signal moveTo(address:string)
    signal push(address:string)
    signal moveForward()
    signal moveBack()

    function clear() {
        __clean_index = -1;
        __command_index = -1;
        __commands = [];
    }

    onPush: function(address) {
        if(__isNew(address))
        {
            if (__command_index !== __commands.length - 1)
                __commands.splice(__command_index + 1, __commands.length);

            __commands.push(address);
            __command_index += 1;
        }
        // Tell recipients to update their models:
        moveTo(address);
    }

    onMoveForward: {
        if (__canMoveForward()) {
            __command_index += 1;

            var address = __commands[__command_index];
            moveTo(address);
        }
    }

    onMoveBack: {
        if (__canMoveBack()) {
            __command_index -= 1;
            var address = __commands[__command_index];
            moveTo(address);
        }
    }

    function setClean() {
        __clean_index = __command_index;
    }

    function __canMoveBack() {
        return __command_index >= 0;
    }

    function __canMoveForward() {
        return __command_index < __commands.length - 1;
    }

    function __isClean() {
        return __command_index === __clean_index;
    }

    function __isNew(address) {
        if(__command_index == 0) {
            return true
        } else {
            return __commands[__command_index-1] != address;
        }
    }
}