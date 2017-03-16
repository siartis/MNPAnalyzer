#ifndef MARKETANALYSISWINDOW_H
#define MARKETANALYSISWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QByteArray>
#include "mainwindow.h"
#include "filehandler.h"
#include <QList>
#include "worker.h"
#include "networkhandler.h"
#include "math.h"
#include <QMovie>

namespace Ui {
class MarketAnalysisWindow;
}

class MarketAnalysisWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MarketAnalysisWindow(const QString & regionName,
                                  QWidget *parent = 0);
    ~MarketAnalysisWindow();
    void closeEvent(QCloseEvent *e);

signals:
    void marketAnalysisWindowClosed(); //Закрытие окна

private slots:
    void on_startButton_clicked();
    void on_getResearchButton_clicked();

    //Свои слоты
    void anotherOperator(); //Другой оператор
    void numberFinished(QString phoneNumber,
                        QString oper,
                        QString region); //Номер обработан
    void getOtherOperators();

    void uploadProgress(qint64 v,
                        qint64 sent,
                        qint64 total); //Прогресс загрузки файла на сервер
    void sentFinished(QString request,
                      QByteArray bbb); //Отправка файла на сервер завершена
    void deleteRow();//Ошибка отработки номера    

    void on_informationButton_clicked();

private:
    Ui::MarketAnalysisWindow *ui;
    FileHandler m_fl;

    QMovie *m_mov; //Прелоадер

    int m_from; //Начальное значение (для перебора по номерам)
    int m_before; //Конечное значение (для перебора по номерам)

    int m_countThread; //Количество потоков
    int m_countNumber; //Количетво изучаемых номеров

    QString m_operatorName; //Название выбранного пользователем оператора
    QString m_operatorCode; //Код оператора

    int m_countAnotherOperator; //Количество других операторов
    int m_countChooseOperator; //Количество исследуемых операторов (из отработанных номеров)
    int m_countProcessedNumber; //Количество обработанных номеров
    int m_countFailedNumber; //Количество необработанных (ошибка 500 сервера) номеров

    QList<QString> m_otherOperators; //Список других операторов

    int m_countOther; //Количество несовпадений с другими операторами

    QList<int> m_countOtherOperators; //Количество других операторов

    int m_currentServerCode;

    QList<QString> m_phoneNumberList; //Массив номеров телефонов
    QList<QString> m_operatorNameList; //Массив названий операторов
    QList<QString> m_operatorRegionNameList; //Регион местонахождения оператора
    QList<int> m_factChange; //Факт смены оператора

    QByteArray m_tableResult; //Таблица с номерами
    int m_mnpServiceCode;

    int m_countFactChangeOperator; //Количество смененных операторов по номерам

    int m_countRangeNumber; //Количество номеров в диапазоне (емкость диапазона номеров)

    QString m_regionName; //Название региона
};

#endif // MARKETANALYSISWINDOW_H
