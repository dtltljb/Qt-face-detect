#include <QtCore/QObject>
#include <QtCore/QStandardPaths>
#include <QThread>

#include "face_alignment_entry.h"
#include "faceAnalysisThread.h"

const std::string DATA_DIR = "../camera_qml/data/";

extern FaceAlignment faceAlignment;
extern FaceAnalysis faceAnalysis;

FaceAnalysis::FaceAnalysis(QObject *parent):
     QObject(parent)
 {
    QThread *thread = new QThread();
    QObject::connect(thread,SIGNAL(started()),this,SLOT(startWaitAnliysisResutl()) );
    QObject::connect(this,SIGNAL(destroyed(QObject*)),thread,SLOT(quit()));
    QObject::connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    this->moveToThread(thread);
    thread->start();
    qDebug() << " startup thread....";
 }

void FaceAnalysis::setAnalysisResult(QString result)
{
    m_analysisResult = result;
    emit this->analysisResultChanged(m_analysisResult);
}

void FaceAnalysis::PhotoCaptureSlot(const QString &msg)
{
    qDebug() << "Called the C++ slot,msg:" << msg;
    this->photo_capture_flag = msg;
}
/**
 * @brief FaceAnalysis::PhotoNotifySlot
 * @param var
 * 人脸录入
 */
void FaceAnalysis::PhotoNotifySlot(const QString &flag)
{
    qDebug() << " Called the C++ slot, flag: " << flag;
    this->photo_notify_flag = flag;
}



void FaceAnalysis::startWaitAnliysisResutl(void)
{
    ///>
    QObject::connect(this,SIGNAL(route_photo_notify_solt(QString)),
                     this,SLOT(PhotoNotifySlot(QString)) );

    int ret;

    while(1)
    {
        if(this->photo_notify_flag.isEmpty()){
            QThread::msleep(1500);
            //emit this->analysisResultChanged("失败,请看屏幕");
        }else{
            if(this->photo_notify_flag == "clicked"){
                ret = faceAlignment.face_alignment(this->photo_image_path,"123");
                if ( ret != 0 ){
                    emit this->analysisResultChanged("失败,请看屏幕");
                    //setAnalysisResult("失败,请看屏幕");
                }
                else{
                    //setAnalysisResult("人脸录入成功.");
                    emit this->analysisResultChanged("人脸录入成功");
                }
                std::string cmd = "rm -r ";
                std::string path = this->photo_image_path.toStdString();
                cmd.append(path.begin(),path.end());
                system(cmd.c_str());
                this->photo_notify_flag.clear();
            }else if(this->photo_notify_flag == "timer"){
                ret = faceAlignment.faceVerification(this->photo_image_path);
                if(ret != 0){
                    QString probleName = "不认识您，请联系管理员";
                    emit this->analysisResultNotify(probleName);
                }else{
                    QString probleName = "李加波 开门成功";
                    emit this->analysisResultNotify(probleName);
                }
                std::string cmd = "rm -r ";
                std::string path = this->photo_image_path.toStdString();
                cmd.append(path.begin(),path.end());
                system(cmd.c_str());
                this->photo_notify_flag.clear();
            }else{
                qDebug()<< "photo_notify_flag is error: " << this->photo_notify_flag;
                this->photo_notify_flag.clear();
                QThread::msleep(100);
            }
        }


//        if(!this->photo_capture_flag.isEmpty()){
//            ret = faceAlignment.face_alignment(this->photo_capture_flag,"123");
//            if ( ret != 0 ){
//                emit this->analysisResultChanged("失败,请看屏幕");
//            }
//            else{
//                emit this->analysisResultChanged("人脸录入成功");
//            }
//            std::string cmd = "rm -r ";
//            std::string path = imgPath.toStdString();
//            cmd.append(path.begin(),path.end());
//            system(cmd.c_str());
//            this->photo_capture_flag.clear();
//        }else if(!this->photo_notify_flag.isEmpty()){
//            ret = faceAlignment.faceVerification(this->photo_notify_flag);
//            if(ret != 0){
//                QString probleName = "不认识您，请联系管理员";
//                emit this->analysisResultNotify(probleName);
//            }else{
//                QString probleName = "李加波 开门成功";
//                emit this->analysisResultNotify(probleName);
//            }

//            std::string cmd = "rm -r ";
//            std::string path = imgPath.toStdString();
//            cmd.append(path.begin(),path.end());
//            system(cmd.c_str());
//            this->photo_notify_flag.clear();

//        }else {
//            qDebug()<< "start wait analysis result thread...";
//        }


    }
}
