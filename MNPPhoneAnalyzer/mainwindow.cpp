#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>

MainWindow::MainWindow(int threadCount,
                       const QString &oper,
                       const QString &operatorCode,
                       int from,
                       int before,
                       const QString &rangeNumbers,
                       const QString &operatorName,
                       const int mnpServiceCode,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Запрет изменения ячеек в таблице
    ui->phoneTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Блокировка возможности выгрузить данные в html/csv (для Android версии)
#ifdef __ANDROID__
    ui->reportDataLabel->setVisible(false);

    ui->unloadCSVButton->setVisible(false);
    ui->unloadCSVButton->setEnabled(false);

    ui->unloadHTMLButton->setVisible(false);
    ui->unloadHTMLButton->setEnabled(false);
#else
    ui->unloadCSVButton->setVisible(false);
    ui->unloadHTMLButton->setVisible(false);
#endif
    //

    //Получение данных с формы настроек
    m_countThread = threadCount; //Количество потоков
    m_operatorCode = operatorCode; //Код оператора
    m_operator = oper;//Название оператора

    m_from = from;
    m_before = before;

    m_countAnotherOperator = 0;
    m_countProcessedNumber = 0;
    m_countChooseOperator = 0;
    m_countFailedNumber = 0;
    m_tableColumn = 0;
    m_otherOperators.clear();

    m_countFactChangeOperator = 0;

    //Диапазон количества номеров
    m_rangeNumbers = rangeNumbers;

    //Название исследуемого оператора
    m_operatorName = operatorName;

    //MNP-сервис
    m_mnpServiceCode = mnpServiceCode;

    //Емкость диапазона номеров
    m_countRangeNumber = 0;

    //Прогрузка стилей приложения
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");
    ui->startButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->getResearchButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->showDiagramButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));

    ui->unloadCSVButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->unloadHTMLButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Старт операции
void MainWindow::on_startButton_clicked()
{
    m_mov = new QMovie(":/style/loader.gif");
    ui->loaderLabel->setMovie(m_mov);
    m_mov->start();
    ui->loaderLabel->setVisible(true);

    ui->getResearchButton->setEnabled(true);
    ui->showDiagramButton->setEnabled(true);

    m_countNumber = (m_before - m_from) + 1;
    m_countRangeNumber = m_before - m_from;

    //Подготовка таблицы для вывода
    ui->phoneTable->clear(); //Очистка таблицы
    ui->phoneTable->setRowCount(m_countNumber);
    ui->phoneTable->setColumnCount(4);

    //Вертикальные заголовки
    QStringList labels;
    labels.append("№ тел.");
    labels.append("Оператор");
    labels.append("Регион");
    labels.append("Факт смены");
    ui->phoneTable->setHorizontalHeaderLabels(labels);

    labels.clear(); //Очистка памяти

    //Задание стиля
    ui->phoneTable->setStyleSheet(m_fl.getStyle(":/style/tableStyle.css"));
    ui->countsLabel->setText("Подготовка...");


    QThread *thread[m_countThread];
    Worker *w[m_countThread];

    m_before = m_from + (m_countNumber/m_countThread); //Расчет конечного значения

    //Создаем необходимое количество потоков и экземпляров класса Worker, которые будут осуществлять проверку номеров
    for(int i = 0; i < m_countThread; i++) {
        w[i] = new Worker(m_operator, m_operatorCode, m_mnpServiceCode, NULL);
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
        connect(w[i], SIGNAL(numberFinish(QString,QString,QString)),
                this, SLOT(numberFinished(QString,QString,QString)));
        connect(w[i], SIGNAL(numberError()), this, SLOT(deleteRow()));
        connect(w[i], SIGNAL(finishForSort()), this, SLOT(finishForSort()));
        connect(w[i], SIGNAL(destroyed(QObject*)), thread[i], SLOT(quit()));
        connect(w[i], SIGNAL(finish()), this, SLOT(onFinished()));
        connect(w[i], SIGNAL(finish()), w[i], SLOT(deleteLater()));
        connect(thread[i], SIGNAL(destroyed(QObject*)), thread[i], SLOT(quit()));
        connect(thread[i], SIGNAL(finished()), thread[i], SLOT(deleteLater()));

        thread[i]->start(QThread::NormalPriority);

        m_from = m_from + (m_countNumber/m_countThread);
        m_before = m_before + (m_countNumber/m_countThread);
    }

    ui->loaderLabel->setVisible(false);
}

//Другой оператор
void MainWindow::anotherOperator()
{
    m_countAnotherOperator++;
}

//Номер обработан
void MainWindow::numberFinished(QString phoneNumber, QString oper, QString region)
{
    m_countProcessedNumber++;

    ui->phoneTable->setItem(m_tableColumn, 0, new QTableWidgetItem(phoneNumber.prepend("+7")));
    ui->phoneTable->setItem(m_tableColumn, 1, new QTableWidgetItem(oper));
    ui->phoneTable->setItem(m_tableColumn, 2, new QTableWidgetItem(region));

    //if (m_operator != oper) {
    if (QString::compare(m_operator, oper, Qt::CaseInsensitive) != 0) {

        m_countFactChangeOperator++;
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

        //Факт смены
        ui->phoneTable->setItem(m_tableColumn, 3, new QTableWidgetItem("+"));
    }
    else {
        m_countChooseOperator++; //Количество операторов исследуемого оператора
        //Факт смены
        ui->phoneTable->setItem(m_tableColumn, 3, new QTableWidgetItem(""));
    }

    m_tableColumn++; //Увеличиваем номер строки таблицы на единицу

    //Обновляем прогресс
    ui->progressBar->setValue((m_countProcessedNumber*100)/m_countNumber);

    ui->countsLabel->setText(QString("%2/%1 номера (-ов) обработано")
                             .arg(m_countNumber)
                             .arg(m_countProcessedNumber));

    ui->countFactChangeOperatortLabel->setText(QString("Кол-во фактов смены оператора - %1 ")
                                               .arg(m_countFactChangeOperator));


    ui->unloadCSVButton->setVisible(true);
    ui->unloadHTMLButton->setVisible(true);
}

//Окончание процесса (для сортировки таблицы)
void MainWindow::finishForSort()
{
    ui->phoneTable->sortItems(0, Qt::AscendingOrder);
}

//Обработка закончена
void MainWindow::onFinished()
{
    //Подгонка размеров таблицы под содержимое
    ui->phoneTable->resizeColumnsToContents();
    ui->phoneTable->resizeRowsToContents();
}

//Получить отчет
void MainWindow::on_getResearchButton_clicked()
{
    m_currentServerCode = 1;

    QString EMail = QInputDialog::getText(this, "MNPPhoneAnalyzer", "Введите адрес электронной почты, \n"
                                                                    "на который будет отправлен отчет \n"
                                                                    "(не включая диаграмму) " , QLineEdit::Normal);
    if (EMail.isEmpty() || EMail.isNull()) {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Вы не указали адрес электронной почты! \nОтчет не был отправлен. ");
    }
    else {
        ui->loaderLabel->setVisible(true);

        ui->countsLabel->text().clear();
        ui->countFailedNumber->text().clear();

        //Для формирования html файла
        m_tableResult.clear();

        //Количество других операторов
        getOtherOperators();

        for(int i = 0; i < ui->phoneTable->rowCount(); i++) {
            if (!ui->phoneTable->model()->index(i, 0).data().toString().isEmpty()) {
                m_tableResult.append(QString("%1;%2;%3;%4 \n")
                                     .arg(ui->phoneTable->model()->index(i, 0).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 1).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 2).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 3).data().toString()));
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
                         .arg(m_operator)
                         .arg(m_countChooseOperator)
                         .arg((m_countChooseOperator * 100)/m_countProcessedNumber));


        //Дополнительная информация
        tableData.append("<p>");
        tableData.append(QString("<font face=\"Tahoma\">Количество обработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countProcessedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество необработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countFailedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество выявленных фактов смены оператора - <b>%1</b></font><br>")
                         .arg(m_countFactChangeOperator));
        tableData.append("</p>");


        //Отправка данных на сервер
        QThread *th = new QThread(this);
        NetworkHandler *nh = new NetworkHandler(m_tableResult,
                                                "",
                                                EMail,
                                                m_rangeNumbers,
                                                m_operatorName,
                                                tableData,
                                                m_currentServerCode,
                                                false,
                                                m_countRangeNumber,
                                                "",
                                                "");
        nh->moveToThread(th);
        th->start(QThread::NormalPriority);
        connect(th, SIGNAL(started()), nh, SLOT(startProcess()));
        connect(th, SIGNAL(finished()), SLOT(deleteLater()));
        connect(nh, SIGNAL(uploadProgress(qint64,qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64,qint64)));
        connect(nh, SIGNAL(finishSent(QString, QByteArray)), this, SLOT(sentFinished(QString, QByteArray)));
    }
}

void MainWindow::uploadProgress(qint64 v, qint64 sent, qint64 total)
{
    ui->progressBar->setValue(v);
    ui->countsLabel->setText(QString("Отправка отчета... (%1 / %2 байт отправлено) ").arg(sent).arg(total));
}

void MainWindow::sentFinished(QString request, QByteArray bbb)
{
    ui->loaderLabel->setVisible(false);
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

//Ошибка обработки номера
void MainWindow::deleteRow()
{
    m_countFailedNumber++;
    m_countNumber--;

    ui->phoneTable->removeRow(ui->phoneTable->rowCount()-1);

    //Обновляем прогресс
    ui->progressBar->setValue((m_countProcessedNumber*100)/m_countNumber);

    //Выводим на экран количество ошибочных номеров
    ui->countFailedNumber->setText(QString("%1 номера (-ов) не обработано ").arg(m_countFailedNumber));
}

//Закрытие окна
void MainWindow::closeEvent(QCloseEvent *e)
{
    emit mainWindowClosed();
    this->close();
}

//Перейти в форму настроек
void MainWindow::on_action_2_triggered()
{
    emit mainWindowClosed();
    this->close();
}

//Показать диаграмму
void MainWindow::on_action_3_triggered()
{
    QStringList otherOperators, countOtherOperators;
    otherOperators.clear();
    countOtherOperators.clear();

    //Получить список других операторов с их количеством
    getOtherOperators();

    for(int i = 0; i < m_otherOperators.size(); i++) {
        otherOperators.append(m_otherOperators.at(i));
        countOtherOperators.append(QString::number(m_countOtherOperators.at(i)));
    }

    otherOperators.append(m_operator);
    countOtherOperators.append(QString::number(m_countChooseOperator));

    m_gw = new GraphicWindow(otherOperators, countOtherOperators, m_countNumber, this);
    connect(m_gw, SIGNAL(getReport(QByteArray)), this, SLOT(getDiagramData(QByteArray)));
    connect(m_gw, SIGNAL(graphicWindowClosed()), this, SLOT(show()));
    this->hide();
    m_gw->showMaximized();
}

//Получить список других операторов с их количеством
void MainWindow::getOtherOperators()
{
    //Обнуление массива
    for(int u = 0; u < m_countOtherOperators.length(); u++) {
        m_countOtherOperators[u] = 0;
    }

    for(int i = 0; i < ui->phoneTable->rowCount(); i++) {
        //Подсчет количества других операторов в сборке
        for(int j = 0; j < m_otherOperators.length(); j++) {
            if (ui->phoneTable->model()->index(i, 1).data().toString() == m_otherOperators.at(j)) {
                m_countOtherOperators[j]++;
            }
        }
    }
}

//Провести исследование для отправки результатов пользователю
void MainWindow::getResearch()
{
    m_currentServerCode = 1;

    QString EMail = QInputDialog::getText(this, "MNPPhoneAnalyzer", "Введите адрес электронной почты" , QLineEdit::Normal);
    if (EMail.isEmpty() || EMail.isNull()) {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Вы не указали адрес электронной почты! ");
    }
    else {
        ui->loaderLabel->setVisible(true);

        //Для формирования html файла
        m_tableResult.clear();

        getOtherOperators();

        for(int i = 0; i < ui->phoneTable->rowCount(); i++) {
            if (!ui->phoneTable->model()->index(i, 0).data().toString().isEmpty()) {
                m_tableResult.append(QString("%1;%2;%3;%4 \n")
                                     .arg(ui->phoneTable->model()->index(i, 0).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 1).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 2).data().toString())
                                     .arg(ui->phoneTable->model()->index(i, 3).data().toString()));
            }
        }

        QString tableData = "<ul>";
        for(int i = 0; i < m_otherOperators.size(); i++) {
            tableData.append(QString("<li><b>%1</b> - %2 (%3%)</li>\n")
                             .arg(m_otherOperators.at(i))
                             .arg(m_countOtherOperators.at(i))
                             .arg((m_countChooseOperator * 100)/m_countProcessedNumber));
        }

        tableData.append(QString("<li><b>%1</b> - %2 (%3%)</li>")
                         .arg(m_operator)
                         .arg(m_countChooseOperator)
                         .arg((m_countChooseOperator * 100)/m_countProcessedNumber));

        tableData.append("</ul>");

        //Дополнительная информация
        tableData.append("<p>");
        tableData.append(QString("<font face=\"Tahoma\">Количество обработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countProcessedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество необработанных номеров - <b>%1</b></font><br>")
                         .arg(m_countFailedNumber));
        tableData.append(QString("<font face=\"Tahoma\">Количество вывленных фактов смены оператора - <b>%1</b></font>")
                         .arg(m_countFactChangeOperator));
        tableData.append("</p>");

        //Отправка данных на сервер
        QThread *th = new QThread(this);
        NetworkHandler *nh = new NetworkHandler(m_tableResult,
                                                m_diargamData,
                                                EMail,
                                                m_rangeNumbers,
                                                m_operatorName,
                                                tableData,
                                                m_currentServerCode,
                                                false,
                                                m_countRangeNumber,
                                                "",
                                                "");
        nh->moveToThread(th);
        th->start(QThread::NormalPriority);
        connect(th, SIGNAL(started()), nh, SLOT(startProcess()));
        connect(th, SIGNAL(finished()), SLOT(deleteLater()));
        connect(nh, SIGNAL(uploadProgress(qint64,qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64,qint64)));
        connect(nh, SIGNAL(finishSent(QString, QByteArray)), this, SLOT(sentFinished(QString, QByteArray)));
    }
}

//Получить данные о диаграмме
void MainWindow::getDiagramData(QByteArray diargamData)
{
    m_gw->close();
    this->showMaximized();
    m_diargamData = diargamData;
    diargamData.clear();

    getResearch();
}

//Выход
void MainWindow::on_action_triggered()
{
    qApp->quit();
}

//Показать диаграмму
void MainWindow::on_showDiagramButton_clicked()
{
    on_action_3_triggered();
}

//Выгрузить в CSV
void MainWindow::on_unloadCSVButton_clicked()
{
    QDate currentDate = QDate::currentDate(); //Текущая дата
    QTime currentTime = QTime::currentTime(); //Текущее время

    QString path = QFileDialog::getSaveFileName(this,
                                                "MNPPhoneAnalyzer",
                                                QString("MNPPhoneAnalyzer_анализ_%3_%1_%2.csv")
                                                .arg(currentDate.toString("dd_MM"))
                                                .arg(currentTime.toString("hh_mm"))
                                                .arg(m_operator),
                                                "*.csv");
    if (path.isEmpty() || path == "") {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Не выбрана папка для выгрузки файла! ");
    }
    else {
        QString resultData;
        resultData.append("Краткий отчет по исследованию смены операторов в выбранном диапазоне номеров \n");
        resultData.append(QString("по состоянию на %1 %2 \n\n")
                          .arg(currentDate.toString("dd.MM.yyyy"))
                          .arg(currentTime.toString("hh:mm")));
        resultData.append(QString("Диапазон исследуемых номеров - %1 \n\n\n")
                          .arg(m_rangeNumbers));

        int countI = 1;
        resultData.append("Номер п/п;");
        resultData.append("Номер телефона;");
        resultData.append("Оператор;");
        resultData.append("Регион;");
        resultData.append("Факт смены оператора");
        resultData.append("\n");

        for(int i = 0; i < ui->phoneTable->rowCount(); i++) {
            resultData.append(QString("%1;").arg(countI));
            for(int j = 0; j < ui->phoneTable->columnCount(); j++) {
                resultData.append(QString("%1;")
                                  .arg(ui->phoneTable->item(i, j)->text()));
            }
            countI++;
            resultData.chop(1);
            resultData.append("\r\n");
        }

        if (m_fl.writeToFile(path, resultData)) {
            resultData.clear();
            QMessageBox::information(this, "MNPPhoneAnalyzer", "Выгрузка завершена! ");
        }
        else {
            QMessageBox::information(this, "MNPPhoneAnalyzer", "Произошла ошибка при выгрузке в CSV файл! \n"
                                                               "Попробуйте еще раз. ");
        }
    }
}

//Выгрузить в HTML
void MainWindow::on_unloadHTMLButton_clicked()
{
    QDate currentDate = QDate::currentDate(); //Текущая дата
    QTime currentTime = QTime::currentTime(); //Текущее время

    QString path = QFileDialog::getSaveFileName(this,
                                                "MNPPHoneAnalyzer",
                                                QString("MNPPhoneAnalyzer_анализ_%3_%1_%2.htm")
                                                .arg(currentDate.toString("dd_MM"))
                                                .arg(currentTime.toString("hh_mm"))
                                                .arg(m_operator),
                                                "*.htm");

    if (path.isEmpty() || path == "") {
        QMessageBox::information(this, "MNPPhoneAnalyzer", "Не выбрана папка для выгрузки файла! ");
    }
    else {
        QString resultData = "<table border=\"1\" width=\"70%\">";
        resultData.append("<tr>");
        resultData.append("<th>Номер п/п</th>"
                          "<th>Номер телефона</th>"
                          "<th>Оператор</th>"
                          "<th>Регион</th>"
                          "<th>Факт смены оператора</th>");
        resultData.append("</tr>");

        int countI = 1;

        for(int i = 0; i < ui->phoneTable->rowCount(); i++) {
            resultData.append("<tr>");
            resultData.append(QString("<td><center>%1</center></td>")
                              .arg(countI));
            for(int j = 0; j < ui->phoneTable->columnCount(); j++) {
                if (j != 3) {
                    resultData.append(QString("<td>%1</td>")
                                      .arg(ui->phoneTable->item(i, j)->text()));
                }
                else {
                    if (ui->phoneTable->item(i, j)->text().trimmed() == "+") {
                        resultData.append(QString("<td style=\"background: red;\"><font color=\"white\"><center>%1</center></font></td>")
                                          .arg(ui->phoneTable->item(i, j)->text()));
                    }
                    else {
                        resultData.append(QString("<td>%1</td>")
                                          .arg(ui->phoneTable->item(i, j)->text()));
                    }
                }
            }
            countI++;
            resultData.append("</tr>");
        }
        resultData.append("</table>");

        QString fileData = m_fl.readHtmlFileTemplate(":/files/reportTemplate/reportTemplate.htm");
        fileData = fileData.replace("$$$reportDateTime$$$", QString("%1 %2")
                                    .arg(currentDate.toString("dd.MM.yyyy"))
                                    .arg(currentTime.toString("hh:mm:ss")));
        fileData = fileData.replace("$$$reportData$$$", resultData);
        fileData = fileData.replace("$$$rangeNumbers$$$", m_rangeNumbers);
        resultData.clear();

        if (m_fl.writeToFile(path, fileData)) {
            QMessageBox::information(this, "MNPPhoneAnalyzer", "Выгрузка завершена! ");
        }
        else {
            QMessageBox::information(this, "MNPPhoneAnalyzer", "Произошла ошибка при выгрузке в HTML файл! \n"
                                                               "Попробуйте еще раз. ");
        }
    }
}
