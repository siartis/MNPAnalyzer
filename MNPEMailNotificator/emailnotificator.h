#ifndef EMAILNOTIFICATOR_H
#define EMAILNOTIFICATOR_H

#include <QObject>

#include "smtp.h"

class EMailNotificator : public QObject
{
    Q_OBJECT
public:
    explicit EMailNotificator(QObject *parent = 0);

    void sendEMailNotification(const QString &to,
                               const QString &subject,
                               const QString &body,
                               QStringList files);

signals:

public slots:

private:
//    void getData(const QString &to,
//                 const QString &subject,
//                 const QString &body,
//                 QStringList files);
//    QString m_to;
//    QString m_subject;
//    QString m_body;
//    QStringList m_files;

    Smtp *m_smtp;

    QString m_userName = "testUser@vzletsamara.ru";
    QString m_userPassword = "testUser%";
    QString m_server = "smtp.mail.ru";
    int m_serverPort = 465;
};

#endif // EMAILNOTIFICATOR_H
