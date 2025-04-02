import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: appWindow
    visible: true
    visibility: Window.Maximized
    title: "Glubschauge"

    MainView {
        anchors.fill: parent
    }
    Shortcut {
        sequence: StandardKey.Quit
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }
}
