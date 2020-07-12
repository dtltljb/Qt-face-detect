#include <QThread>
#include <QObject>

//! user head
//!
#include "src/face_alignment_entry.h"
#include "src/faceAnalysisThread.h"
#include "src/filereader.h"

#include "main.h"

FaceAlignment faceAlignment;
FaceAnalysis faceAnalysis;


int main(int argc, char* argv[])
{
    ///> 注册组建,共享给 qml
    qmlRegisterType<FaceAlignment>("FaceAlignmentEntry.h",1,0,"FaceAlignment");
    qmlRegisterType<FaceAnalysis>("FaceAnalysisThread.h",1,0,"FaceAnalysis");

    QGuiApplication app(argc,argv);
    QQuickView view;
    view.setSource(QUrl(QLatin1String("qrc:///main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QUrl fileName;
    qreal volume = 0.5;
    QQuickItem *rootItem = view.rootObject();
    rootItem->setProperty("fileName",fileName);
    rootItem->setProperty("volume",volume);

    //! \note configure view property values
    FileReader  fileReader;
    view.rootContext()->setContextProperty("fileReader",&fileReader);
    //view.setProperty("fileReader",fileReader);

    //QStringList pwd = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    //QString p = pwd.at(0);


    const QUrl appPath(QUrl::fromLocalFile(app.applicationDirPath()));
    const QStringList pictureLocation = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation );

    const QUrl imagePath = pictureLocation.isEmpty() ? appPath : QUrl::fromLocalFile(pictureLocation.first());
    view.rootContext()->setContextProperty("imagePath",imagePath);

    const QStringList moviesLocation = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
    const QUrl videoPath = moviesLocation.isEmpty() ? appPath : QUrl::fromLocalFile(moviesLocation.first());
    view.rootContext()->setContextProperty("videoPath",videoPath);


//    QObject *btn_effect = rootItem->findChild<QObject*>("btnEffect");  //! find main.qml of btnEffect
//    if (btn_effect){
//        QObject::connect(btn_effect,SIGNAL(clicked()),&app, SLOT(quit()) );
//    }



    // Qt.quit() called in embedded .qml by default only emits
    // quit() signal, so do this (optionally use Qt.exit()).
    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    //! \note configure view parament
    view.setTitle("<< 人 脸 识 别 >>");
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.resize(640, 480);
    view.show();

    // Delay invocation of init until the event loop has started, to work around
    // a GL context issue on Harmattan: without this, we get the following error
    // when the first ShaderEffectItem is created:
    // "QGLShaderProgram::addShader: Program and shader are not associated with same context"
    QMetaObject::invokeMethod(view.rootObject(), "init", Qt::QueuedConnection);

    FaceAnalysis *p_face = &faceAnalysis;

    QObject *obj = rootItem->findChild<QObject*>("dspSignal");
    if (obj){
       QObject::connect(p_face,SIGNAL(analysisResultChanged(QString)),
                                     obj,SIGNAL(qmlAnalysisResultChanged(QString)) );
    }
    QObject *cap_timer = rootItem->findChild<QObject*>("captureTimer");
    if(cap_timer){
        QObject::connect(p_face,SIGNAL(analysisResultNotify(QString)),
                            cap_timer,SIGNAL(qmlAnliysisResultNotify(QString)) );
    }



//    QObject::connect(r_obj,SIGNAL(qmlPhotoCaptureSignal(QString)),
//                    faceAnalysis,SLOT(PhotoCaptureSlot(QString)) );
    ///>
    QObject::connect(rootItem,SIGNAL(qmlPhotoNotifySignal(QString)),
                     p_face,SLOT(PhotoNotifySlot(QString)) );



//    QObject::connect(r_obj,SIGNAL(qmlPhotoNotifySignal(QString)),
//                     faceAnalysis,SIGNAL(route_photo_notify_solt(QString)) );

    return app.exec();
}
