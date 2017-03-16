#ifndef CHECKINTERNET_H
#define CHECKINTERNET_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

class CheckInternet : public QObject
{
    Q_OBJECT
public:
    explicit CheckInternet(const QString &url,
                           const int &count,
                           QObject *parent = 0);

    void checkInternetAbort();

signals:
    void checkFinished(bool result, int count);
    void finish();

private:
    QString m_url; //URL адрес
    int m_count; //Порядковый номер запроса (1 - тест MNP сервиса, 2 - тест почтового сервиса)
    QNetworkReply *m_r;

public slots:
    void checkInternetConnection(); //Проверка интернет-подключения
};

#endif // CHECKINTERNET_H
