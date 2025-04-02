import QtQuick
import QtMultimedia

Rectangle {
    id: cameraHandle
    property alias active: camera.active
    property alias videoOutput: captureSession.videoOutput
    property alias cameraCount: camera.deviceCount
    property alias availableCameraFormats: camera.cameraDevice.videoFormats
    property alias cameraFormatIndex: camera.formatIndex
    property bool cameraShouldBeMirrored: true

    function start() {
        camera.start()
    }

    function stop() {
        camera.stop()
    }

    function switchToNextCameraDevice() {
        camera.switchToNextDevice()
    }

    MediaDevices {
        id: mediaDevices
    }

    CaptureSession {
        id: captureSession
        camera: Camera {
            id: camera
            property alias devices: mediaDevices.videoInputs
            property int deviceCount: devices.length
            property int deviceIndex: 0
            property int formatIndex: 0
            property var bestResolution: Qt.size(1280, 720)
            focusMode: Camera.FocusModeAuto
            Component.onCompleted: {
                if (deviceCount == 0)
                    return

                for (var i = 0; i < deviceCount; i++) {
                    // we prefer front facing camera (2 is Camera.FrontFace)
                    if (devices[i].position === 2)
                        camera.deviceIndex = i
                }

                if (camera.deviceIndex === 0) {
                    cameraHandle.cameraShouldBeMirrored
                            = (camera.cameraDevice.position !== 1) // (1 is Camera.BackFace)
                    camera.formatIndex = findBestFormatIndex(
                                camera.bestResolution)
                }
            }

            onActiveChanged: {
                if (camera.active) {
                    camera.cameraFormat = camera.cameraDevice.videoFormats[camera.formatIndex]
                    cameraHandle.availableCameraFormatsChanged()
                }
            }

            onDeviceIndexChanged: {
                camera.cameraDevice = camera.devices[camera.deviceIndex]
            }
            onCameraDeviceChanged: {
                cameraHandle.cameraShouldBeMirrored
                        = (camera.cameraDevice.position !== 1) // (1 is Camera.BackFace)
                camera.formatIndex = findBestFormatIndex(camera.bestResolution)
            }
            onFormatIndexChanged: {
                camera.cameraFormat = camera.cameraDevice.videoFormats[camera.formatIndex]
            }

            function switchToNextDevice() {
                camera.deviceIndex = (deviceIndex + 1) % deviceCount
            }

            function findBestFormatIndex(resolution) {
                var minDiff = Number.MAX_VALUE
                var minIndex = 0
                var formats = cameraHandle.availableCameraFormats
                for (var i = 0; i < formats.length; i++) {
                    var formatResolution = formats[i].resolution
                    var widthDiff = (formatResolution.width - resolution.width)
                    var heightDiff = (formatResolution.height - resolution.height)
                    var diff = widthDiff * widthDiff + heightDiff * heightDiff
                    if (diff < minDiff) {
                        minDiff = diff
                        minIndex = i
                    }
                }
                return minIndex
            }
        }
    }
}
