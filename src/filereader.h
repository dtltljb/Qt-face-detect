#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>

class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QObject *parent = nullptr);
    Q_INVOKABLE QString readFile(const QString &fileName);

signals:

public slots:
};

#endif // FILEREADER_H
