import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtMultimedia 5.14
import stephanwidor.Glubschauge 1.0

Drawer {
    id: settingsView
    opacity: 0.8

    property ProcessingCameraView cameraView

    RoundButton {
        id: closeButton
        x: parent.width - 1.5 * width
        y: 0.5 * height
        icon.source: "qrc:/icons/clear.svg"
        palette.button: "transparent"
        onClicked: settingsView.close()
    }

    ColumnLayout {
        id: buttonLayout
        anchors.centerIn: parent
        property int marginWidth: 10

        GroupBox {
            title: "Glubsch"
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                rows: 3
                flow: GridLayout.TopToBottom
                Switch {
                    id: eyesDistortSwitch
                    text: "Eyes"
                    position: processingFilter.doEyesDistort? 1 : 0
                    onClicked: processingFilter.doEyesDistort = (position!=0)
                }
                Switch {
                    id: noseDistortSwitch
                    text: "Nose"
                    position: processingFilter.doNoseDistort? 1 : 0
                    onClicked: processingFilter.doNoseDistort = (position!=0)
                }
                Switch {
                    id: mouthDistortSwitch
                    text: "Mouth"
                    position: processingFilter.doMouthDistort? 1 : 0
                    onClicked: processingFilter.doMouthDistort = (position!=0)
                }
                Switch {
                    id: upperHeadDistortSwitch
                    text: "Upper Head"
                    position: processingFilter.doUpperHeadDistort? 1 : 0
                    onClicked: processingFilter.doUpperHeadDistort = (position!=0)
                }
                Switch {
                    id: lowerHeadDistortSwitch
                    text: "Lower Head"
                    position: processingFilter.doLowerHeadDistort? 1 : 0
                    onClicked: processingFilter.doLowerHeadDistort = (position!=0)
                }
                Switch {
                    id: distortAlwaysSwitch
                    text: "Distort Always"
                    position: processingFilter.distortAlways? 1 : 0
                    onClicked: processingFilter.distortAlways = (position!=0)
                }
            }
        }

        GroupBox {
            title: "Show"
            Layout.fillWidth: true
            RowLayout {
                Switch {
                    id: showLandmarksSwitch
                    text: "Facemarks"
                    position: processingFilter.showLandmarks? 1 : 0
                    onClicked: processingFilter.showLandmarks = (position!=0)
                }
                Item {
                    Layout.fillWidth: true
                }
                Switch {
                    id: showFpsSwitch
                    text: "FPS"
                    position: processingFilter.showFps? 1 : 0
                    onClicked: processingFilter.showFps = (position!=0)
                }
            }
        }

        GroupBox {
            title: "Camera"
            Layout.fillWidth: true
            enabled: !processingFilter.streamingToOutputDevice
            RowLayout {
                ComboBox {
                    id: resolutionComboBox
                    property bool fillingList: false
                    model: ListModel {
                        id: model
                    }
                    onCurrentTextChanged: {
                        if(!resolutionComboBox.fillingList)
                            cameraView.setResolution(resolutionComboBox.currentText)
                    }
                    Connections {
                        target: cameraView
                        function onCameraStatusChanged()
                        {
                            if(cameraView.cameraStatus !== Camera.ActiveStatus)
                                return
                            resolutionComboBox.fillingList = true
                            model.clear()
                            var currentResolution = cameraView.resolution.width.toString() + "x" + cameraView.resolution.height.toString()
                            var list = cameraView.availableResolutions()
                            for(var i=0;i<list.length;++i)
                            {
                                var resolution = list[i].width.toString() + "x" + list[i].height.toString()
                                model.append({resolution})
                                if(resolution === currentResolution)
                                    resolutionComboBox.currentIndex = i
                            }
                            resolutionComboBox.fillingList = false
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                RoundButton {
                    icon.source: "qrc:/icons/camera-flip.svg"
                    palette.button: "transparent"
                    visible: QtMultimedia.availableCameras.length > 1
                    onClicked: cameraView.toggleCamera()
                }
            }
        }

        GroupBox {
            title: "Stream"
            Layout.fillWidth: true
            visible: processingFilter.streamingToOutputPossible
            RowLayout {
                Switch {
                    id: streamToOutputDeviceSwitch
                    text: "Stream to "
                    position: processingFilter.streamingToOutputDevice? 1 : 0
                    onClicked: {
                        if(position==0)
                            processingFilter.stopStreamingToOutputDevice()
                        else
                            processingFilter.streamToOutputDevice(outputDeviceEdit.text)
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                Rectangle {
                    color: "white"
                    border.color: "darkgray"
                    border.width: 2
                    width: outputDeviceEdit.width + 10
                    height: outputDeviceEdit.height +10
                    TextEdit {
                        id: outputDeviceEdit
                        anchors.centerIn: parent
                        text: "/dev/video5"
                        enabled: processingFilter.streamToOutputDevice
                    }
                }
            }
        }
    }
}
