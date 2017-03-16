#include "worker.h"

#include <QFile>

Worker::Worker(const QString &oper,
               const QString operatorCode,
               const int mnpServiceCode,
               QObject *parent) : QObject(parent)
{
    getOperatorName(oper);
    getOperatorCode(operatorCode);
    getMNPServiceCode(mnpServiceCode);
}

Worker::~Worker()
{
    m_operator.clear();
    m_operatorCode.clear();
    m_Phone.clear();
    m_mnpServiceCode = 0;
    this->deleteLater();
}

//Получить из конструктора название выбранного оператора
void Worker::getOperatorName(const QString &oper)
{
    m_operator = oper;
}

//Получить из конструктора код оператора
void Worker::getOperatorCode(const QString &operatorCode)
{
    m_operatorCode = operatorCode;
}

//Получить код MNP-Сервиса, выбранного пользователем
void Worker::getMNPServiceCode(const int mnpServiceCode)
{
    m_mnpServiceCode = mnpServiceCode;
}


//Начало процесса
void Worker::startProcess()
{
    QMutex m;
    m_Phone.clear();

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
        QNetworkReply *r;
        QEventLoop p;

        //Проверяем, какой MNP-сервис выбрал пользователь
        switch (m_mnpServiceCode) {

        //Если Tele2
        case 1:

            r = manager.get(QNetworkRequest(QUrl(QString("http://mnp.tele2.ru/gateway.php?%1")
                                                 .arg(m_Phone))));

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
                m_Phone.clear();
            }
            m.unlock();
            delete r;

            break;






            //Если сервис Мегафон
        case 0:
            r = manager.get(QNetworkRequest(QUrl(QString("http://samara.megafon.ru/api/mfn/info?msisdn=+7%1")
                                                 .arg(m_Phone))));

            p.connect(&manager, SIGNAL(finished(QNetworkReply *)), &p, SLOT(quit()));
            p.exec();

            if (!r->error()) {
                QByteArray b = r->readAll().prepend("[").append("]");
                QJsonDocument jsonDoc = QJsonDocument::fromJson(b);
                QJsonArray jsonArr = jsonDoc.array();

                if (jsonDoc.isArray()) {
                    QString operatorName, regionName;

                    foreach (const QJsonValue & value, jsonArr) {
                        QJsonObject o = value.toObject();
                        operatorName = o["operator"].toString()
                                .replace(" и др.", "");

                        regionName = o["region"].toString()
                                .replace("обл.", "область ")
                                .replace("респ.", "республика ")
                                .replace(" и др.", "");
                    }

                    if (QString::compare(operatorName, m_operator, Qt::CaseInsensitive) != 0) {
                        emit anotherOperator();
                    }
                    else {
                    }

                    emit numberFinish(m_Phone, operatorName, regionName);
                }
            }
            else {
                emit numberError();
                m_Phone.clear();
            }
            m.unlock();
            delete r;

            break;

        }
    }


    m.lock();

    //Окончание процесса (для сортировки)
    emit finishForSort();

    //Окончание процесса
    emit finish();

    m.unlock();
}
