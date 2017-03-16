#ifndef MARKETUSERWINDOW_H
#define MARKETUSERWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include "filehandler.h"
#include "marketanalysiswindow.h"
#include "marketworker.h"
#include <QThread>

namespace Ui {
class MarketUserWindow;
}

class MarketUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MarketUserWindow(QWidget *parent = 0);
    ~MarketUserWindow();
    void closeEvent(QCloseEvent *e);

signals:
    void marketUserWindowClosed(); //Закрытие окна

private slots:
    void on_quitButton_clicked();
    void on_operatorBox_currentIndexChanged(int index);
    void on_nextButton_clicked();

private:
    Ui::MarketUserWindow *ui;

    FileHandler m_fl; //Для загрузки стилей приложения
};

#endif // MARKETUSERWINDOW_H
