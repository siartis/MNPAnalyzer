#ifndef SERVERCHECKAVAILABLEWINDOW_H
#define SERVERCHECKAVAILABLEWINDOW_H

#include <QDialog>
#include "checkinternet.h"
#include "welcomewindow.h"
#include "filehandler.h"

#include <QMessageBox>
#include <QThread>
#include <QPixmap>
#include <QMovie>
#include <QTimer>

namespace Ui {
class ServerCheckAvailableWindow;
}

class ServerCheckAvailableWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ServerCheckAvailableWindow(QWidget *parent = 0);
    ~ServerCheckAvailableWindow();

private slots:
    void testInternetFinished(bool result, int count); //Тестирование интернет подключения завершено
    void timerTimeout(); //Отработка одной секунды таймера (ожидания)

    void on_nextButton_clicked();

signals:
    void timerFinish(); //5 секунд ожидания прошло
    void checkInternetAbort(); //Прекратить проверку Интернет-соединения

private:
    Ui::ServerCheckAvailableWindow *ui;

    CheckInternet *m_ci;
    WelcomeWindow *m_ww;
    FileHandler m_fl;

    //Прелоадеры
    QMovie *m_mov1;
    QMovie *m_mov2;

    QTimer *m_timer;

    //Флаги успешности проверки Интернет-соединения с сервисами
    bool m_ok1, m_ok2;

    //Проверка Интернет-соединения
    void checkInternetConnection();

    void setCheckTest(const QString &url, const int &count); //Запуск проверки интернет-тестирования

    int m_sec; //Количество секунд для таймера
};

#endif // SERVERCHECKAVAILABLEWINDOW_H
