import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
import QtMultimedia 5.14
import stephanwidor.Glubschauge 1.0

Rectangle {
    anchors.fill: parent
    id: scanningView

    ProcessingCameraView {
        id: cameraView
        anchors.fill: parent
    }

    RowLayout {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        RoundButton {
            id: playVideoButton
            icon.source: "qrc:/browser-tabs.svg"
            palette.button: "transparent"
            visible: !cameraView.playingVideo
            onClicked: cameraView.playVideo(VideoChooser.chooseVideo())
        }
        Item {
            Layout.fillWidth: true
        }
        RoundButton {
            id: settingsButton
            icon.source: "qrc:/properties.svg"
            palette.button: "transparent"
            onClicked: settingsView.open()
        }
    }

    RowLayout {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20

        RoundButton {
            id: toggleCameraButton
            icon.source: "qrc:/camera-flip.svg"
            palette.button: "transparent"
            visible: !cameraView.playingVideo && QtMultimedia.availableCameras.length > 1
            onClicked: cameraView.toggleCamera()
        }

        Frame {
            visible: cameraView.playingVideo
            Layout.fillWidth: true
            padding: 0
            RowLayout {
                anchors.fill: parent
                RoundButton {
                    id: stopVideoButton
                    icon.source: "qrc:/stop.svg"
                    palette.button: "transparent"
                    visible: cameraView.playingVideo
                    onClicked: cameraView.stopVideo()
                }
                Slider {
                    id: videoSlider
                    visible: cameraView.playingVideo
                    from: 0
                    to: 1
                    value: cameraView.mediaPos
                    Layout.fillWidth: true
                    onMoved: cameraView.setMediaPos(value)
                }
            }
        }

        Item {
            visible: !cameraView.playingVideo
            Layout.fillWidth: true
        }

        RoundButton {
            id: captureButton
            icon.source: "qrc:/camera-symbolic.svg"
            palette.button: "transparent"
            onClicked: processingFilter.capture()
        }

        Item {
            visible: processingFilter.gifEnabled
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
                width: 1.2 * gifButton.width
                height: 1.2 * gifButton.height
                visible: processingFilter.processingGif
                running: processingFilter.processingGif
            }
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
