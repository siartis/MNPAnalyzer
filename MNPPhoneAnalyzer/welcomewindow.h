#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QDialog>

#include "filehandler.h"
#include "settingwindow.h"
#include <QCloseEvent>

#include "checkuserwindow.h"

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = 0);
    ~WelcomeWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_quitButton_clicked();
    void on_nextButton_clicked();

private:
    Ui::WelcomeWindow *ui;
    FileHandler m_fl;
};

#endif // WELCOMEWINDOW_H
