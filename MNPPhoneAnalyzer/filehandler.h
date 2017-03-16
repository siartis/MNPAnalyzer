#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = 0);
    ~FileHandler();

    QString getStyle(const QString &fileName); //Загрузка стилей
    QString getWelcomeMessageText(); //Загрузка приветствия в textBrowser

    bool writeToFile(const QString &path,
                     const QString &textData); //Запись в файл

    QString readHtmlFileTemplate(const QString &path); //Чтение из файла шаблона html-содержимого

private:
    QFile m_file;
};

#endif // FILEHANDLER_H
