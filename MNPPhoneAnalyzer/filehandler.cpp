#include "filehandler.h"

FileHandler::FileHandler(QObject *parent) : QObject(parent) { }
FileHandler::~FileHandler() { }

//Загрузка стилей
QString FileHandler::getStyle(const QString &fileName)
{
    m_file.setFileName(fileName);
    m_file.open(QIODevice::ReadOnly);
    if (m_file.isOpen()) {
        QString result = m_file.readAll();
        m_file.close();

        return result;
    }
    else {
        return "error oper file";
    }
}

//Загрузка текста приветствия в textBrowser
QString FileHandler::getWelcomeMessageText()
{
    m_file.setFileName(":/files/welcome.htm");
    m_file.open(QIODevice::ReadOnly);
    if (m_file.isOpen()) {
        QString result = m_file.readAll();
        m_file.close();

        return result;
    }
    else {
        return "error oper file";
    }
}

//Запись в файл
bool FileHandler::writeToFile(const QString &path,
                              const QString &textData)
{
    m_file.setFileName(path);
    m_file.open(QIODevice::WriteOnly);
    if (!m_file.isOpen()) {
        return false;
    }
    else {
        m_file.write(textData.toLocal8Bit());
        m_file.close();

        return true;
    }
}

//Чтение из файла шаблона html-содержимого
QString FileHandler::readHtmlFileTemplate(const QString &path)
{
    m_file.setFileName(path);
    m_file.open(QIODevice::ReadOnly);
    if (m_file.isOpen()) {
        QString result = QString::fromLocal8Bit(m_file.readAll());
        m_file.close();

        return result;
    }
    else {
        return "error oper file";
    }
}
