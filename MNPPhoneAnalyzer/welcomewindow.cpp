#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QMessageBox>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");
    ui->welcomeBrowser->setStyleSheet(m_fl.getStyle(":/style/tableStyle.css"));

    ui->nextButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->quitButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_quitButton_clicked()
{
    qApp->quit();
}

void WelcomeWindow::on_nextButton_clicked()
{
    CheckUserWindow *cuw = new CheckUserWindow(this);
    connect(cuw, SIGNAL(checkUserWindowClosed()), this, SLOT(showMaximized()));
    cuw->showMaximized();
    this->hide();

}

void WelcomeWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;
    msgBox.setText("Вы действительно хотите закрыть приложение? ");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();

    if(result == QMessageBox::Yes) {
        qApp->quit();
    }
    else {
        e->ignore();
    }
}
