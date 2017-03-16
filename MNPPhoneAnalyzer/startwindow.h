#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>

#include <QMovie>
#include "filehandler.h"
#include <QTimer>
#include "welcomewindow.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private slots:
    void timerFinish();

private:
    Ui::StartWindow *ui;

    FileHandler m_fl; //Для загрузки стилей приложения
    QMovie *m_mov; //Прелоадер
};

#endif // STARTWINDOW_H
