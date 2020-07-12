import QtQuick 2.9
import QtQuick.Window 2.1
import QtMultimedia 5.5
import QtQuick.Controls 2.2
///> user header
import FaceAlignmentEntry.h 1.0
import FaceAnalysisThread.h 1.0

import "qml"
import "qml/button" as P_btn
//import "qml/shared" as Shared


Rectangle {

    id:cameraUI


    width: 640
    height: 480
    //title: qsTr("Hello World")
    color:"black"
    state:"PhotoView"
    visible: true

    objectName: "faceCamera"
    signal qmlPhotoCaptureSignal(string str);
    signal qmlPhotoNotifySignal(string flag);
///
    property int pixDens: Math.ceil(Screen.pixelDensity)
    property int itemWidth: 25 * pixDens
    property int itemHeight: 10 * pixDens
    property int scaledMargin: 2 * pixDens
    property int fontSize: 5 * pixDens
    property string fileName
    property bool perMonitorsLogging: false
    property bool perMonitorsVisible: false
    property int windowWidth: Screen.desktopAvailableWidth
    property int windowHeight: Screen.desktopAvailableHeight
    property string page: " "



    QtObject {
        id: faceObject


        property  int counter:2;
        property string passStr: "hello string"
        property string captureImagePath: camera.imageCapture.capturedImagePath

        Component.onCompleted: {
            faceObject.counter = 2;
            //attrs.debugSignal.connect(onDebug());
           // attrs.passStr = "how are you";
           // countDown.start();
        }

    }

    Camera {
        id:camera
        property string flag: ""
        property string path: ""
        captureMode: Camera.CaptureStillImage
        //viewfinder.resolution:"160x120"
        //viewfinder.resolution:"320x240"
        viewfinder.resolution:"640x480"

        imageCapture {
            id: cameracapture
            onImageCaptured: {
                camera.stop();
                //camera.imageCapture.setResolution(0.2);
                camera.start();
            }

            onImageSaved:{
                path = camera.imageCapture.capturedImagePath;
                cameraUI.qmlPhotoNotifySignal(path);          ///> sender signal to c++
                console.log("camera Captured happen: " + path);
            }
        }

        Component.onCompleted: {
            fileOpen.recordFace.connect(face);
            console.log("deviceId: " + camera.deviceId)
        }
        onCameraStateChanged: {
            //console.log("camera state changed to: " );
        }
    }

    Item {
        id:itemVideo
       // width: 320
        //height: 240
       width: parent.width
       height: parent.height

        VideoOutput {
            id:viewfinder
            source: camera
            anchors.fill: parent
            visible: camera.imageCapture.ready
            focus : visible // to receive focus and capture key events when visible
            MouseArea {
                id:video_click
                anchors.fill: viewfinder
                visible: camera.imageCapture.ready
                onClicked: {
                    camera.flag = "clicked";
                    camera.imageCapture.capture();
                }
            }
            Component.onCompleted: {
                var resolution = camera.viewfinder.resolution;
                console.log("resolution: " + resolution.width + " * " + resolution.height);
            }
        }
    }

    Text{
        id:displayResu

        text:"hello"
        visible: true
        anchors{
            right:parent.right
            bottom: dspResu.top
            margins:scaledMargin
        }

        objectName: "captureTimer"
        signal qmlAnliysisResultNotify(string str);
        onQmlAnliysisResultNotify: {
            countDown.start();
            displayResu.visible = true;
            displayResu.text = str;    ///> anliysis result
            console.log("display result: " + str);
        }
    }

    Text{
        id:dspResu
        text:"world"
        visible: true
        anchors{
            right:parent.right
            bottom: effectName.top
            //top:parent.bottom
            margins:scaledMargin
        }
        objectName: "dspSignal"  ///> findChild
        /// ^^^^^^^^^^^^^^^^^^^^^
        signal qmlAnalysisResultChanged(string result);      ///> declare signal
        /// ^^^^^^^^^^^^^^^
        onQmlAnalysisResultChanged: {                         ///> declare slot Function
        //! ^^^^^^^^^^^^^^^
            dspResu.visible = true;
            dspResu.text = result;
            console.log("analysis changed: " + result);
        }
    }

    Loader{
        id: record

        visible:  true
        source: page
        onLoaded: {
            console.info(" --- loader start ---")
        }
    }

    Button{
        id:effectName
        //objectName:"btnEffect"
        visible: true
        text: "No effect"
        width: itemWidth * 2
        height: itemHeight

        anchors{
            right:parent.right
            bottom: parent.bottom
            margins:scaledMargin
        }
        onClicked: {
            console.log("btnEffect clicked signal...");
            countDown.start(); ///> start timer

        }

    }


    FileOpen {

        id: fileOpen
        state: "collapsed"

        anchors{
            left: parent.left
            top: parent.top
            bottom: parent.bottom //* 0.5
            margins: scaledMargin
        }

        states: [
            State {
                name: "expanded"
                PropertyChanges {
                    target: fileOpen
                    width:itemWidth *1.5
                    opacity: 0.5
                }
            },
            State {
                name: "collapsed"
                PropertyChanges {
                    target: fileOpen
                    width:itemHeight + scaledMargin
                    opacity:0.5

                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation{
                    target: fileOpen; property: "width";duration: 100
                }
                NumberAnimation{
                    target:fileOpen;property: "opacity";duration: 100
                }
            }
        ]


    }


    Timer {
        id:countDown;

        interval: 1000;
        repeat: true;
        triggeredOnStart: true;

        onTriggered: {
            camera.flag = "timer";
            camera.imageCapture.capture();
            countDown.stop();               ///> stop timer , anliysis result open timer
            countDown.interval = 1000;
            dspResu.visible = false;
            console.info(" --- count down run ---")
            faceObject.counter -=1;
            if(faceObject.counter < 0)
            {
                faceObject.counter = 2;
                //countShow.text = "clap now!!";
            }
        }
    }

    function onDebug(){
        console.log("debug to face...");
    }

    function face(fa){
        console.log("face to face..." + fa);
        page = fa;
        record.visible = true;
        viewfinder.visible = false;
    }


    function init(){//c++ invokeMethod this Function
        if (Qt.platform.os == "linux" || Qt.platform.os == "window"){
            if( Screen.desktopAvailableWidth > 1280){
                windowWidth = 1280;
            }
            if(Screen.desktopAvailableHeight > 720){
                windowHeight = 720;
            }
        }
        //countDown.interval = 1000;
        //countDown.start(); ///> start timer
        console.debug(" view system initialize startup...");
    }

}
