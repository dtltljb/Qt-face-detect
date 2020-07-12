import QtQuick 2.0

Rectangle {
    id:button
    height: itemHeight
    width: itemWidth
    opacity: 0.5
    color: "transparent"

    signal clicked

    property string text
    property color bgColor:"transparent"
    property color bgColoSelect:"#14aaff"
    property color textColor:"white"
    property alias enabled:  mouseArea.enabled //如果属性别名与已有的属性名相同，就会把已有的属性覆盖掉
    property bool active:true
    property alias horizontalAlign: text.horizontalAlignment

    Rectangle {
        anchors {fill:parent; margins: 1}
        color: mouseArea.pressed ? bgColoSelect : bgColor

        Text{
            id: text
            clip:true
            text:button.text
            anchors { fill:parent; margins:scaledMargin }
            font.pixelSize: fontSize
            color: textColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

        }

        MouseArea {
            id:mouseArea
            anchors.fill: parent
            onClicked:{
                button.clicked();
            }
            enabled: active

        }
    }

}
