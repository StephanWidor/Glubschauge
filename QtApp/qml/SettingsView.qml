import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
import stephanwidor.Glubschauge 1.0

Drawer {
    id: settingsView
    property CameraHandle cameraHandle

    // not sure why, but this is not properly updated at startup otherwise
    onOpened: cameraFormatComboBox.currentIndex = cameraHandle.cameraFormatIndex

    RoundButton {
        id: closeButton
        x: parent.width - 1.5 * width
        y: 0.5 * height
        icon.source: "qrc:/icons/clear.svg"
        palette.button: "transparent"
        onClicked: {
            processingFilter.saveConfig()
            settingsView.close()
        }
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
                rows: 6
                flow: GridLayout.TopToBottom
                Slider {
                    id: eyesDistortSlider
                    value: processingFilter.eyesDistort
                    onMoved: processingFilter.eyesDistort = position
                }
                Slider {
                    id: noseDistortSlider
                    value: processingFilter.noseDistort
                    onMoved: processingFilter.noseDistort = position
                }
                Slider {
                    id: mouthDistortSlider
                    value: processingFilter.mouthDistort
                    onMoved: processingFilter.mouthDistort = position
                }
                Slider {
                    id: upperHeadDistortSlider
                    value: processingFilter.upperHeadDistort
                    onMoved: processingFilter.upperHeadDistort = position
                }
                Slider {
                    id: lowerHeadDistortSlider
                    value: processingFilter.lowerHeadDistort
                    onMoved: processingFilter.lowerHeadDistort = position
                }
                Switch {
                    id: distortAlwaysSwitch
                    position: processingFilter.distortAlways ? 1 : 0
                    onClicked: processingFilter.distortAlways = (position !== 0)
                }
                Label {
                    text: "Eyes"
                }
                Label {
                    text: "Nose"
                }
                Label {
                    text: "Mouth"
                }
                Label {
                    text: "Upper Head"
                }
                Label {
                    text: "Lower Head"
                }
                Label {
                    text: "Distort Always"
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
                    position: processingFilter.showLandmarks ? 1 : 0
                    onClicked: processingFilter.showLandmarks = (position !== 0)
                }
                Item {
                    Layout.fillWidth: true
                }
                Switch {
                    id: showFpsSwitch
                    text: "FPS"
                    position: processingFilter.showFps ? 1 : 0
                    onClicked: processingFilter.showFps = (position !== 0)
                }
            }
        }

        GroupBox {
            title: "Camera"
            Layout.fillWidth: true
            enabled: !processingFilter.streamingToV4lDevice
            RowLayout {
                ComboBox {
                    id: cameraFormatComboBox
                    textRole: "text"
                    implicitContentWidthPolicy: ComboBox.WidestText
                    model: ListModel {
                        id: formatModel
                    }
                    currentIndex: cameraHandle.cameraFormatIndex

                    onActivated: {
                        if (currentIndex !== cameraHandle.cameraFormatIndex)
                            cameraHandle.cameraFormatIndex = currentIndex
                    }
                    function makeSizeString(size) {
                        return size.width.toString(
                                    ) + "x" + size.height.toString()
                    }
                    function makePixelFormatString(pixelFormat) {
                        const formatStrings = ["Invalid", "ARGB8888", "ARGB8888_Premultiplied", "XRGB8888", "BGRA8888", "BGRA8888_Premultiplied", "BGRX8888", "ABGR8888", "XBGR8888", "RGBA8888", "RGBX8888", "AYUV", "AYUV_Premultiplied", "YUV420P", "YUV422P", "YV12", "UYVY", "YUYV", "NV12", "NV21", "IMC1", "IMC2", "IMC3", "IMC4", "Y8", "Y16", "P010", "P016", "SamplerExternalOES", "Jpeg", "SamplerRect", "YUV420P10"]
                        if (pixelFormat >= formatStrings.length)
                            return "unknown (" + pixelFormat.toString() + ")"
                        return formatStrings[pixelFormat]
                    }
                    function makeCameraFormatText(cameraFormat) {
                        return cameraFormatComboBox.makePixelFormatString(
                                    cameraFormat.pixelFormat) + ", "
                                + cameraFormatComboBox.makeSizeString(
                                    cameraFormat.resolution)
                    }
                    function updateModel() {
                        formatModel.clear()
                        var availableFormats = cameraHandle.availableCameraFormats
                        for (var i = 0; i < availableFormats.length; ++i) {
                            var format = availableFormats[i]
                            formatModel.append({
                                                   "text": cameraFormatComboBox.makeCameraFormatText(
                                                               format)
                                               })
                        }
                    }

                    Connections {
                        target: cameraHandle
                        function onAvailableCameraFormatsChanged() {
                            cameraFormatComboBox.updateModel()
                            cameraFormatComboBox.currentIndex = cameraHandle.cameraFormatIndex
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                RoundButton {
                    icon.source: "qrc:/icons/camera-flip.svg"
                    palette.button: "transparent"
                    visible: cameraHandle.cameraCount > 1
                    onClicked: cameraHandle.switchToNextCameraDevice()
                }
            }
        }

        GroupBox {
            id: v4lGroupBox
            title: "V4L"
            Layout.fillWidth: true
            visible: processingFilter.v4lOutputPossible
            property string toolTipText: "To make this available, do\n'sudo modprobe v4l2loopback exclusive_caps=1 video_nr="
                                         + v4lDeviceComboBox.currentIndex.toString(
                                             ) + " card_label=\"Glubschauge Cam\"'"
            Popup {
                id: v4lErrorPopup
                modal: true
                anchors.centerIn: parent
                background: Rectangle {
                    color: "#e61010"
                }
                Text {
                    text: "Couldn't open " + v4lDeviceComboBox.currentValue
                }
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            }
            RowLayout {
                Switch {
                    text: "Loopback to "
                    position: processingFilter.v4lDeviceActive ? 1 : 0
                    onClicked: {
                        if (position === 0)
                            processingFilter.clearV4lDevice()
                        else {
                            processingFilter.setV4lDevice(
                                        v4lDeviceComboBox.currentValue)
                            if (!processingFilter.v4lDeviceActice) {
                                position = 0
                                v4lErrorPopup.open()
                            }
                        }
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: v4lGroupBox.toolTipText
                }
                Item {
                    Layout.fillWidth: true
                }
                ComboBox {
                    id: v4lDeviceComboBox
                    enabled: !processingFilter.v4lDeviceActice
                    textRole: "path"
                    valueRole: "path"
                    implicitContentWidthPolicy: ComboBox.WidestText
                    model: ListModel {
                        id: v4lModel
                    }
                    Component.onCompleted: {
                        for (var i = 0; i < 10; ++i) {
                            v4lModel.append({
                                                "path": "/dev/video" + i.toString()
                                            })
                        }
                        v4lDeviceComboBox.currentIndex = 5
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: v4lGroupBox.toolTipText
                }
            }
        }
    }
}
