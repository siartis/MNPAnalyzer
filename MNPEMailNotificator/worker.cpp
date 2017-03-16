#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) { }
Worker::~Worker() { }

QString Worker::parsingTableToHTML(const QString &text)
{
    QStringList list = text.split("\n");
    QStringList resultList;
    QString result;
    QString res;

    result.append("</tr>");

    for(int i = 0; i < list.length()-1; i++) {
        resultList = list.at(i).split(";");

        result.append("<tr>");
        result.append(QString("<td align=\"center\">%1</td>").arg(i+1));
        for(int j = 0; j < resultList.length(); j++) {
            res = resultList.at(j);
            if (res.replace(" ", "") == "+") {
                result.append(QString("<td align=\"center\" style=\"color: white; background: red; font-weight: 700;\">%1</td>")
                              .arg(resultList.at(j)));
            }
            else {
                result.append(QString("<td align=\"center\">%1</td>")
                              .arg(resultList.at(j)));
            }
        }
        result.append("</tr>");
    }

    list.clear();
    resultList.clear();

    return result;
}

QString Worker::ByteArrayToString(const QByteArray &textData)
{
    QByteArray b;
    QString result;

    b.append(textData);
    result = QString::fromLocal8Bit(QByteArray::fromBase64(b));

    return result;
}

