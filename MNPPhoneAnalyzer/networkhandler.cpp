#include "networkhandler.h"

NetworkHandler::NetworkHandler(QByteArray tableResult,
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
                               QObject *parent) : QObject(parent)
{
    m_tableData.clear();
    m_tableData.append(tableData);
    tableData.clear();

    m_tableResult.clear();
    m_tableResult = tableResult;
    tableResult.clear();

    if(diagramData.isNull() || diagramData.isEmpty()) {
        noDiagram = true;
    }
    else {
        noDiagram = false;
        m_diagramData.clear();
        m_diagramData = diagramData;
        diagramData.clear();
    }

    m_EMail.clear();
    m_EMail = EMail;
    EMail.clear();

    m_rangeNumbers = rangeNumbers;
    rangeNumbers.clear();

    m_operatorName = operatorName;
    operatorName.clear();

    m_currentServerCode = currentServerCode;

    m_analyst = analyst;

    m_countRangeNumbers = countRangeNumbers;
    m_operatorCode = operatorCode;

    m_regionName = regionName;
}

NetworkHandler::~NetworkHandler()
{
}

void NetworkHandler::startProcess()
{
    m_resultToSent.clear();

    if(noDiagram) {

        if(m_analyst) {
            //Аналитический отчет
            m_resultToSent.append("sendMMPAnalyst");
        }
        else {
            m_resultToSent.append("sendMMPWithoutDiagram");
        }
    }
    else {
        m_resultToSent.append("sendMMP");
    }

    m_resultToSent.append("&");
    m_resultToSent.append(QString("%1&").arg(m_tableResult.length()));

    QByteArray bb;
    bb.append(m_EMail);

    m_resultToSent.append(bb.toBase64());
    m_resultToSent.append("&");
    m_resultToSent.append(m_tableResult.toBase64());

    if (!noDiagram) {
        m_resultToSent.append("&");
        m_resultToSent.append(m_diagramData.toBase64());
    }

    bb.clear();
    bb.append(m_rangeNumbers);
    m_resultToSent.append("&");
    m_resultToSent.append(bb.toBase64()); //Диапазон исследуемых номеров

    bb.clear();
    bb.append(m_operatorName);
    m_resultToSent.append("&");
    m_resultToSent.append(bb.toBase64());
    bb.clear();

    m_resultToSent.append("&");
    m_resultToSent.append(m_tableData.toBase64());

    m_resultToSent.append("&");
    m_resultToSent.append(QString("%1").arg(m_countRangeNumbers));

    m_resultToSent.append("&");
    m_resultToSent.append(m_operatorCode);

    if(m_analyst) {
        bb.clear();
        bb.append(m_regionName);
        m_resultToSent.append("&");
        m_resultToSent.append(bb.toBase64());
    }


    QNetworkAccessManager *am = new QNetworkAccessManager(this);

    //Выбор сервера (пользователем)
    QUrl url;
    if (m_currentServerCode == 1) {
        url.setUrl("http://85.113.47.142:8080/script/mnpm/MNPModule.exe"); //Сервер 1
    }

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("text/html;"));
    reply = am->post(request, m_resultToSent);

    connect(reply, SIGNAL(finished()), this, SLOT(sentFinish()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(upload(qint64,qint64)));
}

//Состояние отправки данных
void NetworkHandler::upload(qint64 sent, qint64 total)
{
    if (sent != 0) {
        emit uploadProgress((sent*100)/total, sent, total);
    }
}

void NetworkHandler::sentFinish()
{
    emit finishSent(reply->readAll(), m_resultToSent);
}
