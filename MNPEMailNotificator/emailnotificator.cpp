#include "emailnotificator.h"

EMailNotificator::EMailNotificator(QObject *parent) : QObject(parent)
{
    //getData(to, subject, body, files);
}

//void EMailNotificator::getData(const QString &to,
//                               const QString &subject,
//                               const QString &body,
//                               QStringList files)
//{
//    m_to = to;
//    m_subject = subject;
//    m_body = body;
//    m_files = files;
//}

void EMailNotificator::sendEMailNotification(const QString &to,
                                             const QString &subject,
                                             const QString &body,
                                             QStringList files)
{
    m_smtp = new Smtp(m_userName, m_userPassword, m_server, m_serverPort);
    m_smtp->sendMail(m_userName, to, subject, body, files);
}

