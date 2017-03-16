#include "marketanalysiswindow.h"
#include "ui_marketanalysiswindow.h"

MarketAnalysisWindow::MarketAnalysisWindow(const QString &regionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarketAnalysisWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей для элементов
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");

    m_countAnotherOperator = 0;
    m_countProcessedNumber = 0;
    m_countChooseOperator = 0;
    m_countFailedNumber = 0;
    m_otherOperators.clear();

    m_operatorNameList.clear();
    m_phoneNumberList.clear();
    m_operatorRegionNameList.clear();
    m_factChange.clear();

    m_countFactChangeOperator = 0;

    m_countRangeNumber = 0;

    m_regionName = regionName;

    ui->mnpServiceBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
    ui->startButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->getResearchButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->serverBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
    ui->threadSpin->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
}

MarketAnalysisWindow::~MarketAnalysisWindow()
{
    delete ui;
}

//Закрытие окна
void MarketAnalysisWindow::closeEvent(QCloseEvent *e)
{
    emit marketAnalysisWindowClosed();
    this->close();
}

//Кнопка "Старт"
void MarketAnalysisWindow::on_startButton_clicked()
{
    //Для Билайн Самарской области
    m_from = 3230000;
    m_before = 3249999;

    m_countRangeNumber = (m_before - m_from) + 1; //Емкость диапазона

    //Общее количество номеров для исследования
    m_countNumber = (m_before - m_from) + 1;

    //Количество потоков - 100
    //m_countThread = 100;
    m_countThread = ui->threadSpin->value();

    m_operatorName = "Билайн";
    m_operatorCode = "903";

    m_mnpServiceCode = ui->mnpServiceBox->currentIndex();

    ui->countsLabel->setText("Подготовка...");

    QThread *thread[m_countThread];
    Worker *w[m_countThread];

    m_before = m_from + (m_countNumber/m_countThread); //Расчет конечного значения

    //Создаем необходимое количество потоков и экземпляров класса Worker, которые будут осуществлять проверку номеров
    for(int i = 0; i < m_countThread; i++) {
        w[i] = new Worker(m_operatorName, m_operatorCode, m_mnpServiceCode, NULL);
        thread[i] = new QThread;

        if (i > 0) {
            w[i]->from = m_from + 1;
            w[i]->before = m_before;
        }
        else {
            w[i]->from = m_from;
            w[i]->before = m_before;
        }

        w[i]->moveToThread(thread[i]);

        connect(w[i], SIGNAL(anotherOperator()), this, SLOT(anotherOperator())); //Другой оператор
        connect(thread[i], SIGNAL(started()), w[i], SLOT(startProcess()));
        connect(w[i], SIGNAL(numberFinish(QString,QString,QString)), this, SLOT(numberFinished(QString,QString,QString)));
        connect(w[i], SIGNAL(numberError()), this, SLOT(deleteRow()));
        connect(w[i], SIGNAL(destroyed(QObject*)), thread[i], SLOT(quit()));
        connect(w[i], SIGNAL(finish()), w[i], SLOT(deleteLater()));
        connect(thread[i], SIGNAL(destroyed(QObject*)), thread[i], SLOT(quit()));
        connect(thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));

        thread[i]->start(QThread::HighPriority);

        m_from = m_from + (m_countNumber/m_countThread);
        m_before = m_before + (m_countNumber/m_countThread);
    }
}

//Другой оператор
void MarketAnalysisWindow::anotherOperator()
{
    m_countAnotherOperator++;
}

//Номер обработан
void MarketAnalysisWindow::numberFinished(QString phoneNumber, QString oper, QString region)
{
    m_countProcessedNumber++;
    m_operatorNameList.append(oper);
    m_phoneNumberList.append(phoneNumber);
    m_operatorRegionNameList.append(region);

    if (QString::compare(m_operatorName, oper, Qt::CaseInsensitive) != 0) {

        m_factChange.append(1);

        //Другой оператор
        m_countOther = 0; //Количество несовпадений с другими операторами

        for(int j = 0; j < m_otherOperators.length(); j++) {
            if (oper != m_otherOperators.at(j)) {
                m_countOther++;
            }
        }

        if (m_countOther == m_otherOperators.length()) {
            m_otherOperators.append(oper); //Добавляем нового другого оператора
            m_countOtherOperators.append(0); //Добавляем 0 (количество) других операторов (по каждому из них)
        }
    }
    else {
        m_factChange.append(0);
        m_countChooseOperator++; //Количество операторов исследуемого оператора
    }

    //Обновляем прогресс
    ui->progressBar->setValue((m_countProcessedNumber*100)/m_countNumber);

    ui->countsLabel->setText(QString("%2/%1 номера (-ов) обработано")
                             .arg(m_countNumber)
                             .arg(m_countProcessedNumber));
}

//Получить список других операторов с их количеством
void MarketAnalysisWindow::getOtherOperators()
{
    QString result;

    //Обнуление массива
    for(int u = 0; u < m_countOtherOperators.length(); u++) {
        m_countOtherOperators[u] = 0;
    }

    for(int i = 0; i < m_phoneNumberList.size(); i++) {
        //Подсчет количества других операторов в сборке
        for(int j = 0; j < m_otherOperators.length(); j++) {
            if (m_operatorNameList.at(i) == m_otherOperators.at(j)) {
                m_countOtherOperators[j]++;
            }
        }

        result.append(QString("%1 - %2 \r\n")
                      .arg(m_phoneNumberList.at(i))
                      .arg(m_operatorNameList.at(i)));
    }
}


//Ошибка обработки номера
void MarketAnalysisWindow::deleteRow()
{
    m_countFailedNumber++;
    m_countNumber--;

    //Обновляем прогресс
    ui->progressBar->setValue((m_countProcessedNumber*100)/m_countNumber);

    //Выводим на экран количество ошибочных номеров
    ui->countFailedNumberLabel->setText(QString("%1 номера (-ов) не обработано ").arg(m_countFailedNumber));
}

void MarketAnalysisWindow::on_getResearchButton_clicked()
{
    m_currentServerCode = 0;
    if (ui->serverBox->currentIndex() == 0) {
        //Выбран сервер 1
        m_currentServerCode = 1;
    }
    else {
        //Выбран сервер 2
        m_currentServerCode = 2;
    }

    QString EMail = QInputDialog::getText(this, "MNPPhoneAnalyzer", "Введите адрес электронной почты, \n"
                                                                    "на который будет отправлен отчет",
                                          QLineEdit::Normal);

    if (EMail.isEmpty() || EMail.isNull()) {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Вы не указали адрес электронной почты! \nОтчет не был отправлен. ");
    }
    else {

        //Для формирования html файла
        m_tableResult.clear();

        //Количество других операторов
        getOtherOperators();

        for(int i = 0; i < m_phoneNumberList.length(); i++) {
            m_tableResult.append(QString("+7%1;%2;%3;")
                                 .arg(m_phoneNumberList.at(i))
                                 .arg(m_operatorNameList.at(i))
                                 .arg(m_operatorRegionNameList.at(i)));

            if (m_factChange.at(i) == 1) {
                m_tableResult.append("+ \n");
            }
            else {
                m_tableResult.append(" \n");
            }
        }


        QString tableData = "<ul>";
        for(int i = 0; i < m_otherOperators.size(); i++) {
            tableData.append(QString("<li><b>%1</b> - %2 (%3%)</li>\n")
                             .arg(m_otherOperators.at(i))
                             .arg(m_countOtherOperators.at(i))
                             .arg((m_countOtherOperators.at(i) * 100)/m_countProcessedNumber));
        }

        tableData.append(QString("<li><b>%1</b> - %2 (%3%)</li>")
                         .arg(m_operatorName)
                         .arg(m_countChooseOperator)
                         .arg((m_countChooseOperator * 100)/m_countProcessedNumber));
        tableData.append("</ul>");


        //Дополнительная информация
        tableData.append("<p>");
        tableData.append(QString("<font face=\"Tahoma\">Количество обработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countProcessedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество необработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countFailedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество выявленных фактов смены оператора - <b>%1</b></font><br>")
                         .arg(m_countAnotherOperator));
        tableData.append("</p>");

        //Отправка данных на сервер
        QThread *th = new QThread(this);
        NetworkHandler *nh = new NetworkHandler(m_tableResult,
                                                "",
                                                EMail,
                                                "+79191234567 - +79191234567",
                                                m_operatorName,
                                                tableData,
                                                m_currentServerCode,
                                                true,
                                                m_countRangeNumber,
                                                m_operatorCode,
                                                m_regionName);

        nh->moveToThread(th);
        th->start(QThread::NormalPriority);
        connect(th, SIGNAL(started()), nh, SLOT(startProcess()));
        connect(th, SIGNAL(finished()), SLOT(deleteLater()));
        connect(nh, SIGNAL(uploadProgress(qint64,qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64,qint64)));
        connect(nh, SIGNAL(finishSent(QString, QByteArray)), this, SLOT(sentFinished(QString, QByteArray)));
    }
}


void MarketAnalysisWindow::uploadProgress(qint64 v, qint64 sent, qint64 total)
{
    ui->progressBar->setValue(v);
    ui->countsLabel->setText(QString("Отправка отчета... (%1 / %2 байт отправлено) ").arg(sent).arg(total));
}

void MarketAnalysisWindow::sentFinished(QString request, QByteArray bbb)
{
    if (request.toInt() == 1) {
        ui->countsLabel->setText("Файл с отчетом был успешно отправлен! ");
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Файл с отчетом был успешно отправлен на указанный адрес электронной почты! ");
    }
    else {
        ui->countsLabel->setText("Ошибка отправки файла с отчетом! ");
        QMessageBox::critical(this, "MNPPhoneAnalyzer", "Ошибка отправки файла с отчетом!\n"
                                                        "Возможные причины:"
                                                        "1) Отсутствует соединение с сетью Интернет;\n"
                                                        "2) Отсутствует связь с почтовым сервисом. \n"
                                                        "Попробуйте повторить попытку позже. ");
    }
}

void MarketAnalysisWindow::on_informationButton_clicked()
{
    QMessageBox::information(this, "MNPPhoneAnalyzer",
                             "Обращаем Ваше внимание на то, что наиболее оптимальным количеством потоков для исследования порядка 1000 номеров будет 50-60 единиц.\n"
                             "Если указать большее количество потоков (максимальное количество - 200), то MNP сервер будет рассчитывать запросы мобильного приложения как DDos атаку и на запрос отвечать внутренней ошибкой сервера, а это, в свою очередь будет означать то, что определенный номер (или группа номеров) не будут обработаны и в конечном итоге не войдут в краткий отчет и результативную диаграмму! ");
}
