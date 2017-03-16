#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    //Установка валидации на ввод только цифр
    ui->codeOperatorLine->setValidator(new QIntValidator(0, 999, ui->codeOperatorLine));
    ui->codeOperatorLine2->setValidator(new QIntValidator(0, 999, ui->codeOperatorLine2));
    ui->phoneLine1->setValidator(new QIntValidator(0, 999, ui->phoneLine1));
    ui->phoneLine2->setValidator(new QIntValidator(0, 999, ui->phoneLine2));
    ui->phoneLine3->setValidator(new QIntValidator(0, 999, ui->phoneLine3));
    ui->phoneLine12->setValidator(new QIntValidator(0, 999, ui->phoneLine12));
    ui->phoneLine22->setValidator(new QIntValidator(0, 999, ui->phoneLine22));
    ui->phoneLine32->setValidator(new QIntValidator(0, 999, ui->phoneLine32));

    //Прогрузка стилей на элементы
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");

    ui->codeOperatorLine->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->codeOperatorLine2->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));

    ui->phoneLine1->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->phoneLine2->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->phoneLine3->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->phoneLine12->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->phoneLine22->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));
    ui->phoneLine32->setStyleSheet(m_fl.getStyle(":/style/lineStyle.css"));

    ui->operatorBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
    ui->threadSpin->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));

    ui->mnpServiceBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));

    ui->nextButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->quitButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::on_codeOperatorLine_textChanged(const QString &arg1)
{
    ui->codeOperatorLine2->setText(arg1);
}

void SettingWindow::on_nextButton_clicked()
{
    QString operatorCode = ui->codeOperatorLine->text(); //Код оператора
    QString oper = ui->operatorBox->currentText(); //Название оператора
    int countThread = ui->threadSpin->value(); //Количество потоков
    int mnpServiceCode = ui->mnpServiceBox->currentIndex();//MNP-сервис

    QString pFrom = QString("%1%2%3")
            .arg(ui->phoneLine1->text())
            .arg(ui->phoneLine2->text())
            .arg(ui->phoneLine3->text());
    int phoneFrom = pFrom.toInt();

    QString pBefore = QString("%1%2%3")
            .arg(ui->phoneLine12->text())
            .arg(ui->phoneLine22->text())
            .arg(ui->phoneLine32->text());
    int phoneBefore = pBefore.toInt();

    //Количество заданных номеров в диапазоне
    if (((phoneBefore - phoneFrom) == 0) || ((phoneBefore - phoneFrom) < 0)) {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Количество номеров в заданном диапазоне не должно быть равным 0 или быть меньше 0!\n"
                                                           "Также обращаем Ваше внимание на то, что установлено ограничение на пределельно допустимое количество исследуемых номеров в диапазоне - 5 000 единиц. ");
    }
    else {        
        //Ограничение по количеству номеров
        if ((phoneBefore - phoneFrom) > limitNumbers) {
            QMessageBox::information(this, "MNPPhoneAnalyzer", QString("Установление ограничение предельно допустимого количества исследуемых номеров - %1 единиц! ")
                                     .arg(limitNumbers));
        }
        else {
            //Диапазон количества номеров
            QString rangeNumbers = QString("+7%1%2%3%4 - +7%5%6%7%8")
                    .arg(ui->codeOperatorLine->text())
                    .arg(ui->phoneLine1->text())
                    .arg(ui->phoneLine2->text())
                    .arg(ui->phoneLine3->text())
                    .arg(ui->codeOperatorLine2->text())
                    .arg(ui->phoneLine12->text())
                    .arg(ui->phoneLine22->text())
                    .arg(ui->phoneLine32->text());

            MainWindow *mw = new MainWindow(countThread,
                                            oper,
                                            operatorCode,
                                            phoneFrom,
                                            phoneBefore,
                                            rangeNumbers,
                                            oper,
                                            mnpServiceCode,
                                            this);
            countThread = 0;
            oper.clear();
            phoneBefore = 0;
            phoneFrom = 0;

            connect(mw, SIGNAL(mainWindowClosed()), this, SLOT(showMaximized()));
            mw->showMaximized();
            this->hide();
        }
    }
}

//Кнопка "Выход"
void SettingWindow::on_quitButton_clicked()
{
    qApp->quit();
}

//Закрытие окна
void SettingWindow::closeEvent(QCloseEvent *e)
{
    emit settingWindowClosed();
    this->close();
}

//Кнопка "information"
void SettingWindow::on_informationButton_clicked()
{
    QMessageBox::information(this, "MNPPhoneAnalyzer",
                             "Обращаем Ваше внимание на то, что наиболее оптимальным количеством потоков для исследования порядка 1000 номеров будет 50-60 единиц.\n"
                             "Если указать большее количество потоков (максимальное количество - 200), то MNP сервер будет рассчитывать запросы мобильного приложения как DDos атаку и на запрос отвечать внутренней ошибкой сервера, а это, в свою очередь будет означать то, что определенный номер (или группа номеров) не будут обработаны и в конечном итоге не войдут в краткий отчет и результативную диаграмму! ");
}
