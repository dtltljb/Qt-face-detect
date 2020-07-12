import QtQuick 2.6
//import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtMultimedia 5.6

Rectangle {
    visible: true

    width: 1280
    height: 960
    color: "black"

    GridLayout {
        id: grid
        rows: 2
        columns: 2

        Item {
            Layout.row: 0
            Layout.column: 0
            Layout.minimumWidth: 80
            Layout.minimumHeight: 30

            Button {
                id: button
                text: "capture"
                onClicked: {
                    camera.stop();
                    camera.viewfinder.resolution = "640x480";
                    camera.start();
                }
            }
        }

        Camera {
            id: camera
            captureMode: Camera.CaptureViewfinder

            viewfinder.resolution: "160x120"

            imageCapture {
                id: cameracapture

                onImageCaptured: {
                    photoPreview.source = preview  // Show the preview in an Image
                    console.log( "capture size: ", photoPreview.sourceSize );
                    timerHelper.restart();
                }

            }

            onCameraStateChanged: {
                console.log("camera state changed to: ", cameraState );
                if ( cameraState == Camera.ActiveState && viewfinder.resolution == Qt.size(640,480) ) {
                    cameracapture.capture();
                }
            }

            function cameraHelper() {
                console.log( "Stopping cam..." );
                camera.stop();
                viewfinder.resolution = "160x120";
                camera.start();
            }
        }

        Timer {
            id: timerHelper
            interval: 1
            onTriggered: camera.cameraHelper();
        }

        Item {
            width: 640
            height: 480

            Layout.row: 1
            Layout.column: 0
            Layout.minimumWidth: 640
            Layout.minimumHeight: 480

            Image {

                width: 640
                height: 480

                id: photoPreview
            }
        }

        Item {
            width: 640
            height: 480

            Layout.row: 1
            Layout.column: 1
            Layout.minimumWidth: 640
            Layout.minimumHeight: 480

            VideoOutput {
                source: camera
                anchors.fill: parent
                focus : visible // to receive focus and capture key events when visible
            }
        }
    }
}
