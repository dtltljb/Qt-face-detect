QT += quick multimedia
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/filereader.cpp \
    src/faceAnalysisThread.cpp \
    src/face_alignment_entry.cpp \
    src/mat_to_qimage.cpp \
    src/forlder_util.cpp



#include($$PWD/snippets/performancemonitor/performancemonitordeclarative.pri)
include($$PWD/src/)
#include($$PWD/shared/quick_shared.qrc)

RESOURCES += qml.qrc
#       ./shared/quick_shared.qrc

HEADERS += \
    main.h \
    src/filereader.h \
    src/faceAnalysisThread.h \
    src/face_alignment_entry.h \
    src/mat_to_qimage.h


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =/home/robot/Qt5.9.6/5.9.6/gcc_64/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# sdl libs 使用此库 qmlquick 的库就装载失败,原因不详
#unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lSDL

#INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

#openCV
INCLUDEPATH += $$PWD/../../../../usr/local/opencv3.4.6/include
DEPENDPATH += $$PWD/../../../../usr/local/opencv3.4.6/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_core

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_highgui

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_imgproc

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_imgcodecs

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_shape

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_video

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv3.4.6/lib/ -lopencv_videoio

#seetaFaceEngine libs
unix:!macx: LIBS += -L$$PWD/../../thirdLibs/SeetaFaceEngine/FaceAlignment/build/ -lseeta_fa_lib

INCLUDEPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceAlignment/include
DEPENDPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceAlignment/include

unix:!macx: LIBS += -L$$PWD/../../thirdLibs/SeetaFaceEngine/FaceDetection/build/ -lseeta_facedet_lib

INCLUDEPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceDetection/include
DEPENDPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceDetection/include

unix:!macx: LIBS += -L$$PWD/../../thirdLibs/SeetaFaceEngine/FaceIdentification/build/ -lviplnet

INCLUDEPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceIdentification/include
DEPENDPATH += $$PWD/../../thirdLibs/SeetaFaceEngine/FaceIdentification/include



# Default rules for deployment.

#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES +=



