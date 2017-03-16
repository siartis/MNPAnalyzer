#include "servercheckavailablewindow.h"
#include "ui_servercheckavailablewindow.h"

ServerCheckAvailableWindow::ServerCheckAvailableWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerCheckAvailableWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");

    ui->nextButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->nextButton->setVisible(false);
    ui->checkLabel->setVisible(false);

    m_timer = new QTimer();
    m_sec = 0;

    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), SLOT(timerTimeout()));
    connect(this, SIGNAL(timerFinish()), m_timer, SLOT(deleteLater()));
    m_timer->start();

    ui->checkPostServerLabel->setVisible(false);
    ui->loaderLabel2->setVisible(false);

    //Запуск прелоадеров
    m_mov1 = new QMovie(":/style/loader.gif");
    ui->loaderLabel1->setMovie(m_mov1);
    m_mov1->start();

    m_mov2 = new QMovie(":/style/loader.gif");
    ui->loaderLabel2->setMovie(m_mov2);
    m_mov2->start();

    //Проверка Интернет-соединения
    checkInternetConnection();
}

ServerCheckAvailableWindow::~ServerCheckAvailableWindow()
{
    delete ui;
}

//Запуск проверки интернет-тестирования
void ServerCheckAvailableWindow::setCheckTest(const QString &url, const int &count)
{
    m_ci = new CheckInternet(url, count);
    QThread *th = new QThread(this);
    m_ci->moveToThread(th);
    th->start(QThread::NormalPriority);
    connect(th, SIGNAL(started()), m_ci, SLOT(checkInternetConnection()));
    connect(m_ci, SIGNAL(checkFinished(bool,int)), this, SLOT(testInternetFinished(bool, int)));
    connect(m_ci, SIGNAL(finish()), m_ci, SLOT(deleteLater()));
    connect(th, SIGNAL(finished()), SLOT(deleteLater()));
    connect(this, SIGNAL(checkInternetAbort()), m_ci, SLOT(deleteLater()));
}

//Проверка Интернет-соединения
void ServerCheckAvailableWindow::checkInternetConnection()
{
    setCheckTest("http://mnp.tele2.ru", 1);
}

//Отработка одной секунды таймера (ожидания)
void ServerCheckAvailableWindow::timerTimeout()
{
    m_sec++;
    if (m_sec == 7) {
        m_timer->stop();
        ui->checkLabel->setVisible(true);
        ui->nextButton->setVisible(true);
        emit timerFinish();
    }
}

//Тестирование интернет подключения завершено
void ServerCheckAvailableWindow::testInternetFinished(bool result, const int count)
{
    QPixmap p;

    if (count == 1) {
        if (result) {
            m_ok1 = true;
            p.load(":/style/okImage.png");
            ui->loaderLabel1->setPixmap(p);
            ui->loaderLabel1->setGeometry(ui->loaderLabel1->x(), ui->loaderLabel1->y(), 31, 31);
        }
        else {
            m_ok1 = false;
            p.load(":/style/errorImage.png");
            ui->loaderLabel1->setPixmap(p);
            ui->loaderLabel1->setGeometry(ui->loaderLabel1->x(), ui->loaderLabel1->y(), 31, 31);
        }

        //Продолжение тестирования
        ui->checkPostServerLabel->setVisible(true);
        ui->loaderLabel2->setVisible(true);

        setCheckTest("http://85.113.47.142:8080/script/mnpm/MNPModule.exe?testConnection", 2);
    }
    else {
        if (result) {
            m_ok2 = true;
            p.load(":/style/okImage.png");
            ui->loaderLabel2->setPixmap(p);
            ui->loaderLabel2->setGeometry(ui->loaderLabel2->x(), ui->loaderLabel2->y(), 31, 31);
        }
        else {
            m_ok2 = false;
            p.load(":/style/errorImage.png");
            ui->loaderLabel2->setPixmap(p);
            ui->loaderLabel2->setGeometry(ui->loaderLabel2->x(), ui->loaderLabel2->y(), 31, 31);
        }

        if (m_ok1 && m_ok2) {

            m_timer->stop();
            emit timerFinish();

            m_ww = new WelcomeWindow();
            this->close();
            m_ww->showMaximized();
        }
        else {
            if (!m_ok1) {
                QMessageBox::information(this, "MNPPhoneAnalyzer", "ВНИМАНИЕ! \nНа данный момент не доступен MNP сервис! ");
            }

            if (!m_ok2) {
                QMessageBox::information(this, "MNPPhoneAnalyzer", "ВНИМАНИЕ! \nНа данный момент не доступен почтовый сервис! ");
            }

            m_ww = new WelcomeWindow();
            this->close();
            m_ww->showMaximized();
        }
    }
}

void ServerCheckAvailableWindow::on_nextButton_clicked()
{
    m_timer->stop();
    m_ci->checkInternetAbort();
    emit checkInternetAbort();

    m_ww = new WelcomeWindow(this);
    this->hide();
    m_ww->showMaximized();
}
