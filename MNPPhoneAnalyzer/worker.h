#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonValue>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(const QString &oper,
                    const QString operatorCode,
                    const int mnpServiceCode,
                    QObject *parent = 0);
    ~Worker();

    int from; //Значение от
    int before; //Значение до

private:
    QString m_operator;
    QString m_operatorCode;
    void getOperatorName(const QString &oper); //Получить из конструктора название выбранного оператора
    void getOperatorCode(const QString &operatorCode); //Получить из конструктора код оператора
    QString m_Phone; //Номер телефона (без кода оператора)

    void getMNPServiceCode(const int mnpServiceCode); //Получить код выбранного пользователем MNP-сервиса
    int m_mnpServiceCode;

signals:
    void anotherOperator(); //Другой оператор
    void finish(); //Окончание процесса
    void finishForSort(); //Окончание процесса (для сортировки)
    void numberFinish(QString phoneNumber, QString oper, QString region); //Номер обработан
    void numberError(); //Ошибка отработки номера

public slots:
    void startProcess(); //Начало процесса
};

#endif // WORKER_H
