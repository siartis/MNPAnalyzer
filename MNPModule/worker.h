#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include <QStringList>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

    QString parsingTableToHTML(const QString &text);
    QString ByteArrayToString(const QByteArray &textData);

};

#endif // WORKER_H
