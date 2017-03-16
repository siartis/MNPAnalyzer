#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>

class FileHAndler : public QObject
{
    Q_OBJECT
public:
    explicit FileHAndler(QObject *parent = 0);
    ~FileHAndler();

    QString readFile(const QString &path);
    void writeFile(const QString &path, const QString &fileData);

};

#endif // FILEHANDLER_H
