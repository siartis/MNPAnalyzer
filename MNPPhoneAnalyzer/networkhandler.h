#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include <QFile>

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHandler(QByteArray tableResult,
                            QByteArray diagramData,
                            QString EMail,
                            QString rangeNumbers,
                            QString operatorName,
                            QString tableData,
                            int currentServerCode,
                            bool analyst,
                            int countRangeNumbers,
                            QString operatorCode,
                            QString regionName,
                            QObject *parent = 0);
    ~NetworkHandler();

signals:
    void uploadProgress(qint64 v, qint64 sent, qint64 total);
    void finishSent(QString req, QByteArray bbb);


public slots:
    void startProcess(); //Начало процесса
    void upload(qint64 sent, qint64 total);
    void sentFinish();

private:
    QNetworkReply *reply;
    QByteArray m_resultToSent; //Для отправки
    QByteArray m_diagramData; //Данные диаграммы
    QString m_EMail; //Адрес электронной почты
    bool noDiagram; //Признак - "без диаграммы"
    QString m_rangeNumbers; //Диапазон количества исследуемых номеров
    QString m_operatorName; //Название исследуемого оператора
    QByteArray m_tableResult; //Таблица с номерами (исследование)
    QByteArray m_tableData; //Данные об исследовании - сколько каких операторов в выборке
    int m_currentServerCode; //Номер выбранного пользователем сервера
    bool m_analyst; //Отчет для аналитика или для другого пользователя
    int m_countRangeNumbers; //Количество номеров в диапазоне (емкость диапазона)
    QString m_operatorCode; //Код исследуемого оператора
    QString m_regionName; //Название выбранного пользователем региона
};

#endif // NETWORKHANDLER_H
