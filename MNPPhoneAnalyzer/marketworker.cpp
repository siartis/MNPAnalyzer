#include "marketworker.h"
#include <QMutex>
#include <QFile>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

marketWorker::marketWorker(const QString &oper, const QString operatorCode, QObject *parent) : QObject(parent)
{
    getOperatorName(oper);
    getOperatorCode(operatorCode);
}

marketWorker::~marketWorker()
{
    m_operator.clear();
    m_operatorCode.clear();
    m_Phone.clear();
    this->deleteLater();
}

//Получить из конструктора название выбранного оператора
void marketWorker::getOperatorName(const QString &oper)
{
    m_operator = oper;
}

//Получить из конструктора код оператора
void marketWorker::getOperatorCode(const QString &operatorCode)
{
    m_operatorCode = operatorCode;
}


//Начало процесса
void marketWorker::startProcess()
{
    QMutex m;
    m_Phone = "";

    for(int i = from; i <= before; i++) {
        m.lock();

        m_Phone = m_operatorCode;

        if (QString("%1").arg(i).length() < 7) {
            for(int j = 0; j < 7-QString("%1").arg(i).length(); j++) {
                m_Phone.append("0");
            }
        }

        m_Phone.append(QString("%1").arg(i));

        QNetworkAccessManager manager;
        QNetworkReply *r = manager.get(QNetworkRequest(QUrl(QString("http://mnp.tele2.ru/gateway.php?%1")
                                                            .arg(m_Phone))));

        QEventLoop p;
        p.connect(&manager, SIGNAL(finished(QNetworkReply *)), &p, SLOT(quit()));
        p.exec();

        if (!r->error()) {
            QByteArray b = r->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(b);
            QJsonObject obj = jsonDoc.object();

            //т.к. объекты вложенные
            QJsonObject obj0 = jsonDoc.object(); //Объект JSON
            QJsonObject response = obj0["response"].toObject(); //Ответ (массив)

            if (response.length() != 0) {
                QJsonObject geocodeJ = response["geocode"].toObject(); //Область (массив)
                QString geocode = geocodeJ["value"].toString(); //Область (значение)
                QJsonObject mncJ = response["mnc"].toObject(); //Оператор (массив)
                QString mnc = mncJ["value"].toString(); //Оператор (значение)

                if (mnc != m_operator) {
                    emit anotherOperator();
                }
                else {
                }

                emit numberFinish(m_Phone, mnc, geocode);
            }
        }
        else {
            emit numberError();
            m_Phone = "";
        }
        m.unlock();

        delete r;
    }

    m.lock();

    //Окончание процесса (для сортировки)
    emit finishForSort();

    //Окончание процесса
    emit finish();

    m.unlock();
}


