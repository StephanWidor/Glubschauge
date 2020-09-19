import QtQuick 2.14
import QtMultimedia 5.14
import stephanwidor.Glubschauge 1.0

Rectangle {
    id: cameraView

    property alias contentRect: videoOutput.contentRect
    property alias cameraStatus: camera.cameraStatus
    property size resolution: camera.viewfinder.resolution
    property bool playingVideo: false

    function setResolution(resolution)
    {
        camera.stop()
        camera.viewfinder.resolution = resolution
        camera.start()
    }

    function availableResolutions()
    {
        return camera.supportedViewfinderResolutions()
    }

    function playVideo(file)
    {
        console.log("playVideo: " + file)
        mediaPlayer.source = file
        mediaPlayer.play()
    }

    function stopVideo()
    {
        mediaPlayer.stop()
    }

    function toggleCamera()
    {
        if(camera.position === Camera.FrontFace)
            camera.position = Camera.BackFace
        else
            camera.position = Camera.FrontFace
    }

    Camera {
        id: camera
        position: Camera.FrontFace
        exposure.exposureMode: Camera.ExposureAuto
        focus.focusMode: Camera.FocusContinuous
        viewfinder.onResolutionChanged: console.log("camera resolution: " + viewfinder.resolution)
        onError: console.log(errorString)
    }

    MediaPlayer {
        id: mediaPlayer
        onError: console.log(errorString)
        onPlaying: {
            camera.stop()
            videoOutput.source = mediaPlayer
            videoOutput.autoOrientation = false
            videoOutput.orientation = 0
            cameraView.playingVideo = true
        }
        onStopped: {
            videoOutput.autoOrientation = true
            cameraView.playingVideo = false
            camera.start()
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: camera
        autoOrientation: true
        focus: visible
        filters: [processingFilter]
    }

    Binding {
        target: processingFilter
        property: "rotation"
        value: videoOutput.orientation
    }
    Binding {
        target: processingFilter
        property: "doCameraTransform"
        value: !cameraView.playingVideo
    }
}
