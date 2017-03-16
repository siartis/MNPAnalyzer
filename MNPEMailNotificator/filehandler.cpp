#include "filehandler.h"

FileHAndler::FileHAndler(QObject *parent) : QObject(parent) { }
FileHAndler::~FileHAndler() { }

QString FileHAndler::readFile(const QString &path)
{
    QString result;

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly);
    result = QString::fromLocal8Bit(file.readAll());
    file.close();

    return result;
}

void FileHAndler::writeFile(const QString &path, const QString &fileData)
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::WriteOnly);
    file.write(fileData.toLocal8Bit());
    file.close();
}

