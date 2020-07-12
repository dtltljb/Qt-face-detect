
#ifndef faceAnalysisThread_H
#define faceAnalysisThread_H

#include "qqml.h"

#include <QtQuick/QQuickItem>

#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QEventLoop>
#include <QtCore/QEventTransition>

class FaceAnalysis : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString analysisResult READ getAnalysisResult WRITE setAnalysisResult NOTIFY analysisResultChanged)

public:
    explicit FaceAnalysis(QObject *parent = 0);
    void setAnalysisResult(QString result);

    Q_INVOKABLE QString getAnalysisResult(){
        return this->m_analysisResult;
    }
    //analysisResult = "hello world";

signals:
    void analysisResultChanged(QString result);
    void analysisResultNotify(QString result);

    void route_photo_notify_solt(const QString &flag);

public slots:
    // This is a slot for QString only
    void PhotoCaptureSlot(const QString &msg);
    // This is the slot for a generic data type
    void PhotoNotifySlot(const QString &flag);

    void startWaitAnliysisResutl(void);


protected:
    QString m_analysisResult = "hello world";
    QString photo_notify_flag ;
    QString photo_image_path ;
    QString photo_capture_flag ;
//    bool event(QEvent *event);
//    void timerEvent(QTimerEvent *evnet);

};

//QML_DECLARE_TYPEINFO(FaceAnalysis, QML_HAS_ATTACHED_PROPERTIES)

#endif // MYCLASS_H
