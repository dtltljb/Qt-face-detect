# 基于开源的SeetaFace人脸识别引擎的人脸识别工程。
# 识别库seeta_fa_v1.1.bin代码超出100M没有上传到model文件，自行下载。
Github开源项目: https://github.com/seetaface/SeetaFaceEngine

# 工程依赖库,参考qt工程文档 (camera_qml.pro) 如下：

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




