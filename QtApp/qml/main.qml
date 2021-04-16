import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    id: appWindow
    visible: true
    width: 800
    height: 700
    title: qsTr("Glubschauge")

    ScanningView {
        id: scanningView
        anchors.fill: parent
    }

    Shortcut {
        sequence: StandardKey.Quit
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }
}
