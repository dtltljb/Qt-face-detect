/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick 2.1
import "button"

Rectangle {
    id: root
    color: "#151515"
    signal recordFace(string ss);
    signal mergeDing(var u_object);
    signal checkList(string list);
    signal close();


    Rectangle {//left-top side menu
        id: menuField
        height: itemHeight
        width: itemHeight
        color: "transparent"
        anchors.right: parent.right
        Image {
            id: menu
            source: "qrc:///images/icon_Menu.png"
            anchors {
                right: parent.right
                top: parent.top
                margins: scaledMargin
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: fileOpen.state == "expanded" ? fileOpen.state = "collapsed" : fileOpen.state = "expanded"
        }
    }

    Column {
        anchors {
            top: menuField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom
            topMargin: 10
        }

        spacing: 10
        visible: fileOpen.state == "expanded"

        Rectangle {
            width: 0.9 * parent.width
            height: 1
            color: "#353535"
            anchors.left: parent.left
        }
        Button {
            text: " > 人 脸 录 入"
            height: itemHeight
            width: parent.width
            color: "green"
            onClicked: {
                fileOpen.state = "collapsed";
                root.recordFace("qrc:///qml/listview/expandingdelegates.qml");
                console.log("emit recordFace signal..");
            }
        }
        Rectangle {
            width: 0.9 * parent.width
            height: 1
            color: "#353535"
            anchors.left: parent.left
        }
        Button {
            text: " > 绑 定 钉 钉"
            height: itemHeight
            width: parent.width
            color: "yellow"
            onClicked: {
                fileOpen.state = "collapsed"
                var ob = menuField;
                root.mergeDing(ob);
            }
        }
        Rectangle {
            width: 0.9 * parent.width
            height: 1
            color: "#353535"
            anchors.left: parent.left
        }
        Button {
            text: " > 查 看 考 勤"
            height: itemHeight
            width: parent.width
            color: "blue"
            onClicked: {
                fileOpen.state = "collapsed";
                root.checkList("list");
            }
        }
        Rectangle {
            width: 0.9 * parent.width
            height: 1
            color: "#353535"
            anchors.left: parent.left
        }
        Button {
            text: " > 闭     合"
            height: itemHeight
            width: parent.width
            color:"red"
            onClicked: {
                fileOpen.state = "collapsed";
                root.close();
            }
        }
        Rectangle {
            width: 0.9 * parent.width
            height: 1
            color: "#353535"
            anchors.left: parent.left
        }
    }
}
