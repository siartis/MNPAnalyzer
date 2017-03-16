#include <QCoreApplication>

#include "stdlib.h"
#include "stdio.h"
#include <QByteArray>
#include <QFile>
#include <QTextCodec>
#include <fcntl.h>
#include "io.h"
#include <QStringList>
#include <QProcess>
#include <QDate>
#include <QTime>
#include <QPixmap>

#include "filehandler.h"
#include "worker.h"


QByteArray Input(void) {
    QFile f;
    if(!f.open(fileno(stdin),QIODevice::ReadOnly)) {
        qApp->quit();
        exit(404);
    }
    return f.readAll();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    setmode(fileno(stdout),O_BINARY);
    setmode(fileno(stdin ),O_BINARY);

    QString ps = Input();
    QStringList params = ps.split("&");

    FileHAndler fl;
    Worker w;

    //Отправка результирующего отчета
    if (params.at(0) == "sendMMP") {

        bool ok = true;
        if (ok) {

            QByteArray email;
            email.append(params.at(2));

            QByteArray b;
            b.clear();
            b.append(params.at(3));

            QString diagram = params.at(4);

            //Диапазон исследуемых номеров
            QString rangeNumbers = params.at(5);

            //Название исследуемого оператора
            QString operatorName = params.at(6);

            //Формируем Web-страницу
            QString reportFile = fl.readFile(":/files/report.htm");
            reportFile = reportFile.replace("$$$currentDate$$$", QDate::currentDate().toString("dd.MM.yyyy"));
            reportFile = reportFile.replace("$$$currentTime$$$", QTime::currentTime().toString("hh:mm:ss"));
            reportFile = reportFile.replace("$$$operatorName$$$", w.ByteArrayToString(operatorName.toLocal8Bit()));
            reportFile = reportFile.replace("$$$rangeNumbers$$$", w.ByteArrayToString(rangeNumbers.toLocal8Bit()));
            reportFile = reportFile.replace("$$$table$$$", w.parsingTableToHTML(QString::fromLocal8Bit(QByteArray::fromBase64(b)))); //Формирование таблицы

            b.clear();
            b.append(params.at(7));

            QString tableData;
            tableData.append("<ul>");
            tableData.append(QString::fromLocal8Bit(QByteArray::fromBase64(b)));

            reportFile = reportFile.replace("$$$tableData$$$", tableData);

            QString countRangeNumber = params.at(8); //Емкость диапазона
            reportFile = reportFile.replace("$$$countRangeNumber$$$", countRangeNumber);


            QString dateTime = QString("%1_%2")
                    .arg(QDate::currentDate().toString("ddMMyyyy"))
                    .arg(QTime::currentTime().toString("hhmmss"));


            reportFile = reportFile.replace("$$$diagramData$$$", QString("<img width=\"100%\" height=\"100%\" src=\"%1.png\" />")
                                            .arg(dateTime));

            //Создаем на диске файл из полученных данных
            QString filePath = QString("D:/MNPEMailNotificator/files/%1.htm").arg(dateTime);
            fl.writeFile(filePath, reportFile);

            QString diagramPath = QString("D:/MNPEMailNotificator/files/%1_%2.png")
                    .arg(QDate::currentDate().toString("ddMMyyyy"))
                    .arg(QTime::currentTime().toString("hhmmss"));

            QImage img;
            img.loadFromData(QByteArray::fromBase64(diagram.toLocal8Bit()), "PNG");
            img.save(diagramPath, "PNG");

            b.clear();
            b.append(filePath);

            QProcess p;
            QStringList parametres;
            parametres.clear();

            parametres.append("sentMNP");
            parametres.append(b.toBase64());

            b.clear();
            b.append(diagramPath);
            parametres.append(b.toBase64());
            parametres.append(email);
            parametres.append(rangeNumbers);
            parametres.append(operatorName);

            //Отправка уведомлений
            p.start("D:/MNPEMailNotificator/MNPEMailNotificator.exe", parametres);
            p.waitForReadyRead(20 * 1000);
            int result = p.readAll().toInt();

            switch (result) {
            case 1:
                printf("1");
                a.quit();
                return (200);
                break;
            case -1:
                printf("0");
                a.quit();
                return (200);
                break;

            default:
                printf("0");
                a.quit();
                return (200);
            }

        }
        else {
            printf("error number params");
            a.quit();
            return (200);
        }
    }



    //Отправка только отчета (без диаграммы)
    if (params.at(0) == "sendMMPWithoutDiagram") {
        bool ok = true;
        if (ok) {

            QByteArray email;
            email.append(params.at(2));

            QByteArray b;
            b.clear();
            b.append(params.at(3));

            QString rangeNumbers = params.at(4);
            QString operatorName = params.at(5);


            //Формируем Web-страницу
            QString reportFile = fl.readFile(":/files/report.htm");
            reportFile = reportFile.replace("$$$currentDate$$$", QDate::currentDate().toString("dd.MM.yyyy"));
            reportFile = reportFile.replace("$$$currentTime$$$", QTime::currentTime().toString("hh:mm:ss"));
            reportFile = reportFile.replace("$$$operatorName$$$", w.ByteArrayToString(operatorName.toLocal8Bit()));
            reportFile = reportFile.replace("$$$rangeNumbers$$$", w.ByteArrayToString(rangeNumbers.toLocal8Bit()));
            reportFile = reportFile.replace("$$$table$$$", w.parsingTableToHTML(QString::fromLocal8Bit(QByteArray::fromBase64(b)))); //Таблица

            b.clear();
            b.append(params.at(6));

            QString countRangeNumber = params.at(7); //Емкость диапазона
            reportFile = reportFile.replace("$$$countRangeNumber$$$", countRangeNumber);


            QString tableData;
            tableData.append("<ul>");
            tableData.append(QString::fromLocal8Bit(QByteArray::fromBase64(b)));

            reportFile = reportFile.replace("$$$tableData$$$", tableData);
            reportFile = reportFile.replace("$$$diagramData$$$", "");

            //Создаем на диске файл из полученных данных
            QString filePath = QString("D:/MNPEMailNotificator/files/%1_%2.htm")
                    .arg(QDate::currentDate().toString("ddMMyyyy"))
                    .arg(QTime::currentTime().toString("hhmmss"));
            fl.writeFile(filePath, reportFile);

            b.clear();
            b.append(filePath);

            QProcess p;
            QStringList parametres;
            parametres.clear();

            parametres.append("sentMNPWithoutDiagram");
            parametres.append(b.toBase64());
            parametres.append(email);
            parametres.append(rangeNumbers);
            parametres.append(operatorName);

            //Отправка уведомлений
            p.start("D:/MNPEMailNotificator/MNPEMailNotificator.exe", parametres);
            p.waitForReadyRead(20 * 1000);
            int result = p.readAll().toInt();

            switch (result) {
            case 1:
                printf("1");
                a.quit();
                return (200);
                break;
            case -1:
                printf("0");
                a.quit();
                return (200);
                break;

            default:
                printf("0");
                a.quit();
                return (200);
            }

        }
        else {
            printf("error number params");
            a.quit();
            return (200);
        }
    }






    //Отправка только аналитического отчета (без диаграммы)
    if (params.at(0) == "sendMMPAnalyst") {
        QByteArray email;
        email.append(params.at(2));

        QByteArray b;
        b.clear();
        b.append(params.at(3));

        QString rangeNumbers = params.at(4);
        QString operatorName = params.at(5);


        //Формируем Web-страницу
        QString reportFile = fl.readFile(":/files/analystReport.htm");
        reportFile = reportFile.replace("$$$currentDate$$$", QDate::currentDate().toString("dd.MM.yyyy"));
        reportFile = reportFile.replace("$$$currentTime$$$", QTime::currentTime().toString("hh:mm:ss"));
        reportFile = reportFile.replace("$$$operatorName$$$", w.ByteArrayToString(operatorName.toLocal8Bit()));
        reportFile = reportFile.replace("$$$rangeNumbers$$$", w.ByteArrayToString(rangeNumbers.toLocal8Bit()));
        reportFile = reportFile.replace("$$$table$$$", w.parsingTableToHTML(QString::fromLocal8Bit(QByteArray::fromBase64(b)))); //Таблица

        b.clear();
        b.append(params.at(6));

        QString countRangeNumbers = params.at(7); //Количество номеров в диапазоне (емкость номеров)
        QString operatorCode = params.at(8); //Код оператора

        QString tableData;
        tableData.append("<ul>");
        tableData.append(QString::fromLocal8Bit(QByteArray::fromBase64(b)));

        reportFile = reportFile.replace("$$$tableData$$$", tableData);
        reportFile = reportFile.replace("$$$diagramData$$$", "");

        reportFile = reportFile.replace("$$$countNumbers$$$", countRangeNumbers);
        reportFile = reportFile.replace("$$$operatorCode$$$", operatorCode);

        b.clear();
        b.append(params.at(9));
        QString regionName = QString::fromLocal8Bit(QByteArray::fromBase64(b));
        reportFile = reportFile.replace("$$$regionName$$$", regionName);

        b.clear();

        //Создаем на диске файл из полученных данных
        QString filePath = QString("D:/MNPEMailNotificator/files/analyst_%1_%2.htm")
                .arg(QDate::currentDate().toString("ddMMyyyy"))
                .arg(QTime::currentTime().toString("hhmmss"));
        fl.writeFile(filePath, reportFile);

        b.clear();
        b.append(filePath);

        QProcess p;
        QStringList parametres;
        parametres.clear();

        parametres.append("sentMNPAnalyst");
        parametres.append(b.toBase64());
        parametres.append(email);
        parametres.append(rangeNumbers);
        parametres.append(operatorName);

        b.clear();
        b.append(countRangeNumbers);
        parametres.append(b.toBase64());

        b.clear();
        b.append(operatorCode);
        parametres.append(b.toBase64());

        b.clear();

        //Отправка уведомлений
        p.start("D:/MNPEMailNotificator/MNPEMailNotificator.exe", parametres);
        p.waitForReadyRead(20 * 1000);
        int result = p.readAll().toInt();

        switch (result) {
        case 1:
            printf("1");
            a.quit();
            return (200);
            break;
        case -1:
            printf("0");
            a.quit();
            return (200);
            break;

        default:
            printf("0");
            a.quit();
            return (200);
        }

    }


    a.quit();
    return 200;
}
