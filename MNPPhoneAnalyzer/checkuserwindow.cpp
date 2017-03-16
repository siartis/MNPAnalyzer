#include "checkuserwindow.h"
#include "ui_checkuserwindow.h"

CheckUserWindow::CheckUserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckUserWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");
    ui->studentButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->marketButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
}

CheckUserWindow::~CheckUserWindow()
{
    delete ui;
}

//Закрытие окна
void CheckUserWindow::closeEvent(QCloseEvent *e)
{
    emit checkUserWindowClosed();
    this->close();
}

void CheckUserWindow::on_studentButton_clicked()
{
    SettingWindow *sw = new SettingWindow(this);
    connect(sw, SIGNAL(settingWindowClosed()), this, SLOT(showMaximized()));
    sw->showMaximized();
    this->hide();
}

void CheckUserWindow::on_marketButton_clicked()
{
    MarketUserWindow *muw = new MarketUserWindow(this);
    connect(muw, SIGNAL(marketUserWindowClosed()), this, SLOT(showMaximized()));
    muw->showMaximized();
    this->hide();
}
