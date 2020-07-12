#include "filereader.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

FileReader::FileReader(QObject *parent) : QObject(parent)
{

}

QString FileReader::readFile(const QString &fileName)
{
    QString content;
    QFile file(fileName);

    if ( file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        content = stream.readAll();

    }
    return content;
}
