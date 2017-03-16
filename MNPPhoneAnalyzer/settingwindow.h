#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "filehandler.h"
#include "welcomewindow.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = 0);
    ~SettingWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_codeOperatorLine_textChanged(const QString &arg1);
    void on_nextButton_clicked(); //Кнопка "Далее"
    void on_quitButton_clicked(); //Кнопка "Выход"
    void on_informationButton_clicked(); //Кнопка "information"

signals:
    void settingWindowClosed(); //Закрытие окна

private:
    Ui::SettingWindow *ui;

    FileHandler m_fl;
    bool m_showFirst; //Показывается ли впервые это окно

#ifdef __ANDROID__
    const int limitNumbers = 5 * 1000;
#else
    const int limitNumbers = 100 * 1000;
#endif
};

#endif // SETTINGWINDOW_H
