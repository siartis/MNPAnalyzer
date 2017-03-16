#include "checkinternet.h"

CheckInternet::CheckInternet(const QString &url,
                             const int &count,
                             QObject *parent) : QObject(parent)
{
    m_url = url;
    m_count = count;
}

//Проверка Интернет-подключения
void CheckInternet::checkInternetConnection()
{
    QNetworkAccessManager manager;
    m_r = manager.get(QNetworkRequest(QUrl(m_url)));

    QEventLoop p;
    p.connect(&manager, SIGNAL(finished(QNetworkReply *)), &p, SLOT(quit()));
    p.exec();

    if (!m_r->error()) {
        emit checkFinished(true, m_count);
    }
    else {
        emit checkFinished(false, m_count);
    }

    emit finish();
}

void CheckInternet::checkInternetAbort()
{
    m_r->abort();
}
