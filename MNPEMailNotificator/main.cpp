#include <QCoreApplication>

#include "emailnotificator.h"
#include "filehandler.h"

//Перевод строки из Base64 кодировки в текст
QByteArray getDataFromBase64(const QString text)
{
    QByteArray b;
    b.clear();
    b.append(text);

    return QByteArray::fromBase64(b);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dirPath = "D:/MNPEMailNotificator/template";
    EMailNotificator emailNotificator;
    FileHandler fileHandler;

    //Для отправки файла на почту
    if (argv[1] == "sentMNP")
    {
        QString textMessage = fileHandler.readHtmlFileTemplate(QString("%1/template.htm")
                                                               .arg(dirPath));

        QString filePath = getDataFromBase64(QString("%1").arg(argv[2])); //Excel файл
        QString diagramPath = getDataFromBase64(QString("%1").arg(argv[3]));; //Диаграмма
        QString email = getDataFromBase64(QString("%1").arg(argv[4]));; //Адрес электронной почты пользователя
        QString rangeNumbers = getDataFromBase64(QString("%1").arg(argv[5]));; //Диапазон исследуемых номеров
        QString operatorName = getDataFromBase64(QString("%1").arg(argv[6]));; //Название исследуемого оператора

        textMessage = textMessage.replace("$$$rangeNumbers$$$", rangeNumbers);
        textMessage = textMessage.replace("$$$operatorName$$$", operatorName);
        textMessage = textMessage.replace("$$$Additionally$$$", "<p style=\"margin-right: 100px; margin-left:30px\" align=\"left\">Также в этом письме Вы найдете полученную в ходе исследования диаграмму, отражающую относительную долю (в %) присутствия каждого оператора мобильной связи в выбранном диапазоне. Рекомендуем скачивать оба файла или одним архивом, чтобы диаграмма корректно отображалась в html файле</p>");

        QStringList files;
        files.append(filePath);
        files.append(diagramPath);

        emailNotificator.sendEMailNotification(email,
                                               "Краткий отчет от мобильного приложения \"MNPPhoneAnalyzer\"",
                                               textMessage,
                                               files);

        printf("1");
        a.quit();
        return 0;

        //        EMailNotifier em = new EMailNotifier();
        //        bool ok = em.sendNotification(email, textMessage, "Краткий отчет от мобильного приложения \"MNPPhoneAnalyzer\"", filePath, diagramPath);
        //        if (ok)
        //        {
        //            Console.Write("1");
        //        }
        //        else
        //        {
        //            Console.WriteLine("-1");
        //        }
    }


    //Отправка только отчета (без диаграммы)
    if (argv[1] == "sentMNPWithoutDiagram")
    {
        QString textMessage = fileHandler.readHtmlFileTemplate(QString("%1/template.htm")
                                                               .arg(dirPath));

        QString filePath = getDataFromBase64(QString("%1").arg(argv[2])); //Excel файл
        QString email = getDataFromBase64(QString("%1").arg(argv[3])); //Адрес электронной почты пользователя
        QString rangeNumbers = getDataFromBase64(QString("%1").arg(argv[4])); //Диапазон исследуемых номеров
        QString operatorName = getDataFromBase64(QString("%1").arg(argv[5]));; //Название исследуемого оператора

        textMessage = textMessage.replace("$$$rangeNumbers$$$", rangeNumbers);
        textMessage = textMessage.replace("$$$operatorName$$$", operatorName);
        textMessage = textMessage.replace("$$$Additionally$$$", "");

        //                    EMailNotifier em = new EMailNotifier();
        //                    bool ok = em.sendNotificationWithoutDiagram(email, textMessage, "Краткий отчет от мобильного приложения \"MNPPhoneAnalyzer\"", filePath);
        //                    if (ok)
        //                    {
        //                        Console.Write("1");
        //                    }
        //                    else
        //                    {
        //                        Console.WriteLine("-1");
        //                    }

        QStringList files;
        files.append(filePath);

        emailNotificator.sendEMailNotification(email,
                                               "Краткий отчет от мобильного приложения \"MNPPhoneAnalyzer\"",
                                               textMessage,
                                               files);

        printf("1");
        a.quit();
        return 0;
    }

    //Отправка аналитического отчета (без диаграммы)
    if (argv[1] == "sentMNPAnalyst")
    {
        QString textMessage = fileHandler.readHtmlFileTemplate(QString("%1/templateAnalyst.htm")
                                                               .arg(dirPath));

        QString filePath = getDataFromBase64(QString("%1").arg(argv[2])); //Excel файл
        QString email = getDataFromBase64(QString("%1").arg(argv[3])); //Адрес электронной почты пользователя
        QString rangeNumbers = getDataFromBase64(QString("%1").arg(argv[4])); //Диапазон исследуемых номеров
        QString operatorName = getDataFromBase64(QString("%1").arg(argv[5])); //Название исследуемого оператора
        QString countNumbers = getDataFromBase64(QString("%1").arg(argv[6])); //Емкость диапазона
        QString operatorCode = getDataFromBase64(QString("%1").arg(argv[7])); //Код оператора

        textMessage = textMessage.replace("$$$rangeNumbers$$$", rangeNumbers);
        textMessage = textMessage.replace("$$$operatorName$$$", operatorName);
        textMessage = textMessage.replace("$$$countNumbers$$$", countNumbers);
        textMessage = textMessage.replace("$$$operatorCode$$$", operatorCode);
        textMessage = textMessage.replace("$$$Additionally$$$", "");

        //                    EMailNotifier em = new EMailNotifier();
        //                    bool ok = em.sendNotificationWithoutDiagram(email, textMessage, "Краткий аналитический отчет от мобильного приложения \"MNPPhoneAnalyzer\"", filePath);

        //                    if (ok)
        //                    {
        //                        Console.Write("1");
        //                    }
        //                    else
        //                    {
        //                        Console.WriteLine("-1");
        //                    }


        QStringList files;
        files.append(filePath);

        emailNotificator.sendEMailNotification(email,
                                               "Краткий аналитический отчет от мобильного приложения \"MNPPhoneAnalyzer\"",
                                               textMessage,
                                               files);

        printf("1");
        a.quit();
        return 0;
    }


    a.quit();
    return 0;
    //return a.exec();
}
