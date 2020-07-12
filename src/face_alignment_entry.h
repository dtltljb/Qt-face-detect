#ifndef FACE_ALIGNMENT_ENTRY_H
#define FACE_ALIGNMENT_ENTRY_H
#include <string>
#include <QObject>
#include <QString>
#include <QDebug>

extern std::string DATA_LIB;
extern std::string MODEL_LIB;

//int faceVerification(QString f_name);

class FaceAlignment: public QObject{
    Q_OBJECT
    public:
    FaceAlignment(QObject *parent = 0){
        qDebug("-- create face alignment object startup..");
    }
    signals:
        void faceAlignmentResult( QString res);
        void faceVerificationResult(QString res);
    public slots:
        void done(){
            qDebug() << "faceAlignment::done() called";
        }

        int face_alignment(QString argv,QString number);
        int faceVerification( QString f_name);



};
#endif // FACE_ALIGNMENT_ENTRY_H
