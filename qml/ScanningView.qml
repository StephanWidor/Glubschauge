import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
import QtMultimedia 5.14
import stephanwidor.Glubschauge 1.0

Rectangle {
    anchors.fill: parent
    id: scanningView
    property bool switchesAtBottom: scanningView.height > cameraView.contentRect.height

    ProcessingCameraView {
        id: cameraView
        anchors.fill: parent
    }

    RoundButton {
        id: playVideoButton
        x: 0.5 * width
        y: 0.5 * height
        icon.source: cameraView.playingVideo? "qrc:/camcorder.svg" : "qrc:/browser-tabs.svg"
        palette.button: "transparent"
        onClicked: {
            if(cameraView.playingVideo)
                cameraView.stopVideo()
            else
                cameraView.playVideo(VideoChooser.chooseVideo())
        }
    }

    RoundButton {
        id: settingsButton
        x: parent.width - 1.5 * width
        y: 0.5 * height
        icon.source: "qrc:/properties.svg"
        palette.button: "transparent"
        onClicked: settingsView.open()
    }

    RoundButton {
        id: toggleCameraButton
        x: 0.5 * width
        y: parent.height - 1.5 * height
        icon.source: "qrc:/camera-flip.svg"
        palette.button: "transparent"
        visible: QtMultimedia.availableCameras.length > 1
        onClicked: cameraView.toggleCamera()
    }

    RoundButton {
        id: captureButton
        x: parent.width - 1.5 * width
        y: parent.height - 1.5 * height
        icon.source: "qrc:/camera-symbolic.svg"
        palette.button: "transparent"
        onClicked: processingFilter.capture()
    }

    Item {
        x: captureButton.x - 1.5 * gifButton.width
        y: parent.height - 1.5 * gifButton.height
        width: gifButton.width
        height: gifButton.height
        RoundButton {
            id: gifButton
            enabled: !processingFilter.capturingGif
            text: "gif"
            palette.button: enabled? "transparent" : "red"
            onClicked: processingFilter.captureGif()
        }
        BusyIndicator {
            anchors.centerIn: parent
            width: 1.5 * gifButton.width
            height: 1.5 * gifButton.height
            visible: processingFilter.processingGif
            running: processingFilter.processingGif
        }
    }

    SettingsView {
        id: settingsView
        edge: Qt.TopEdge
        width: parent.width
        height: parent.height
        cameraView: cameraView
    }
}
