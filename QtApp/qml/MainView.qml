import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtMultimedia
import stephanwidor.Glubschauge 1.0

Rectangle {
    anchors.fill: parent
    id: mainView

    states: [
        State {
            name: "UsingCamera"
            StateChangeScript {
                script: {
                    mediaPlayer.stop()
                    cameraHandle.videoOutput = processingFilter
                    cameraHandle.start()
                }
            }
        },
        State {
            name: "ShowingVideo"
            StateChangeScript {
                script: {
                    cameraHandle.stop()
                    mediaPlayer.videoOutput = processingFilter
                    mediaPlayer.play()
                }
            }
        }
    ]

    Component.onCompleted: {
        mainView.state = "UsingCamera"
    }

    Shortcut {
        sequence: StandardKey.Cancel
        context: Qt.ApplicationShortcut
        onActivated: settingsView.opened ? settingsView.close(
                                               ) : mainView.state = "UsingCamera"
    }

    CameraHandle {
        id: cameraHandle
        anchors.fill: parent
    }

    Binding {
        target: processingFilter
        property: "outputMirrored"
        value: cameraHandle.cameraShouldBeMirrored
               && mainView.state === "UsingCamera"
    }

    MediaPlayer {
        id: mediaPlayer
        audioOutput: AudioOutput {
            id: audio
            volume: 1.0
        }
    }

    VideoOutput {
        id: videoOutput
        visible: true
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
        Component.onCompleted: {
            processingFilter.setOutputSink(videoOutput.videoSink)
        }
        Component.onDestruction: {
            processingFilter.setOutputSink(null)
        }
    }

    FileDialog {
        id: openMediaDialog
        title: "Open Video"
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.MoviesLocation)
        selectedNameFilter.index: 0
        nameFilters: ["Videos (*.3gp *.avi *.m4v *.mov *.mp4 *.mpg  *.ogg *.vob *.wmv)"]
        onAccepted: {
            mediaPlayer.source = openMediaDialog.selectedFile
            mainView.state = "ShowingVideo"
        }
    }

    RowLayout {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        RoundButton {
            id: openMediaButton
            icon.source: "qrc:/icons/browser-tabs.svg"
            palette.button: "transparent"
            visible: mainView.state === "UsingCamera"
            onClicked: openMediaDialog.open()
        }
        RoundButton {
            id: closeMediaButton
            icon.source: "qrc:/icons/clear.svg"
            palette.button: "transparent"
            visible: mainView.state !== "UsingCamera"
            onClicked: {
                mainView.state = "UsingCamera"
            }
        }
        Item {
            Layout.fillWidth: true
        }
        RoundButton {
            id: settingsButton
            icon.source: "qrc:/icons/properties.svg"
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
            icon.source: "qrc:/icons/camera-flip.svg"
            palette.button: "transparent"
            visible: cameraHandle.cameraCount > 1
            onClicked: cameraHandle.switchToNextCameraDevice()
        }

        Item {
            Layout.fillWidth: true
            visible: mainView.state !== "ShowingVideo"
        }

        Frame {
            id: videoControl
            Layout.fillWidth: true
            visible: mainView.state === "ShowingVideo"
            RowLayout {
                anchors.fill: parent
                RoundButton {
                    id: startStopButton
                    icon.source: mediaPlayer.playing ? "qrc:/icons/media-playback-pause.svg" : "qrc:/icons/media-playback-start.svg"
                    palette.button: "transparent"
                    onClicked: {
                        if (mediaPlayer.playing)
                            mediaPlayer.pause()
                        else
                            mediaPlayer.play()
                    }
                }
                Slider {
                    id: videoSlider
                    from: 0
                    to: mediaPlayer.duration
                    value: mediaPlayer.position
                    Layout.fillWidth: true
                    onMoved: {
                        if (mediaPlayer.seekable)
                            mediaPlayer.setPosition(value)
                    }
                }
            }
        }

        RoundButton {
            id: captureButton
            icon.source: "qrc:/icons/camera-symbolic.svg"
            palette.button: "transparent"
            enabled: !processingFilter.busyCapture
            onClicked: processingFilter.capture(ProcessingFilter.JPG)
        }
        Item {
            visible: processingFilter.gifEnabled
            width: gifButton.width
            height: gifButton.height
            RoundButton {
                id: gifButton
                enabled: !processingFilter.busyCapture
                text: "gif"
                palette.button: "transparent"
                onClicked: processingFilter.capture(ProcessingFilter.GIF)
            }
            BusyIndicator {
                anchors.centerIn: parent
                width: 1.2 * gifButton.width
                height: 1.2 * gifButton.height
                visible: processingFilter.processingCapture
                running: processingFilter.processingCapture
            }
        }
    }

    SettingsView {
        id: settingsView
        edge: Qt.TopEdge
        width: parent.width
        height: parent.height
        opacity: 0.5
        cameraHandle: cameraHandle
    }
}
