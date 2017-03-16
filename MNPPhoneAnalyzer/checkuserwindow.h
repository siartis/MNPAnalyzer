#ifndef CHECKUSERWINDOW_H
#define CHECKUSERWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include "filehandler.h"
#include "settingwindow.h"
#include "marketuserwindow.h"

namespace Ui {
class CheckUserWindow;
}

class CheckUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CheckUserWindow(QWidget *parent = 0);
    ~CheckUserWindow();
    void closeEvent(QCloseEvent *e);

signals:
    void checkUserWindowClosed(); //Закрытие окна

private slots:
    void on_studentButton_clicked();
    void on_marketButton_clicked();

private:
    Ui::CheckUserWindow *ui;

    FileHandler m_fl;
};

#endif // CHECKUSERWINDOW_H
