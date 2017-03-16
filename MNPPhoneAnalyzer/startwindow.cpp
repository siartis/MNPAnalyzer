#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей приложения
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");

    ui->loaderLabel->setVisible(false);
    m_mov = new QMovie(":/style/startLoader.gif");
    ui->loaderLabel->setMovie(m_mov);
    m_mov->start();
    ui->loaderLabel->setVisible(true);

    QTimer::singleShot(3 * 1000, this, SLOT(timerFinish()));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::timerFinish()
{
    WelcomeWindow *ww = new WelcomeWindow(NULL);
    ww->showMaximized();
    this->close();
}
