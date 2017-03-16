#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QStringList>
#include <QList>
#include <QCloseEvent>
#include <QMovie>

#ifdef __ANDROID__
#else
#include <QFileDialog>
#endif

#include <QThread>
#include <QMessageBox>
#include <QInputDialog>
#include <QTime>
#include <QDate>

#include "filehandler.h"
#include "worker.h"
#include "graphicwindow.h"
#include "settingwindow.h"
#include "networkhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int threadCount,
                        const QString &oper,
                        const QString &operatorCode,
                        const int from,
                        const int before,
                        const QString &rangeNumbers,
                        const QString &operatorName,
                        const int mnpServiceCode,
                        QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_startButton_clicked();
    void on_getResearchButton_clicked();

    void anotherOperator(); //Другой оператор
    void numberFinished(QString phoneNumber,
                        QString oper,
                        QString region); //Номер обработан
    void finishForSort(); //Окончание процесса (для сортировки таблицы)

    void uploadProgress(qint64 v,
                        qint64 sent,
                        qint64 total); //Прогресс загрузки файла на сервер
    void sentFinished(QString request,
                      QByteArray bbb); //Отправка файла на сервер завершена
    void deleteRow();//Ошибка отработки номера
    void onFinished(); //Обработка закончена


    //Элементы меню
    void on_action_triggered(); //Выход
    void on_action_2_triggered(); //Настройки
    void on_action_3_triggered(); //Показать диаграмму
    void getDiagramData(QByteArray diargamData); //Получить данные диаграммы

    void on_showDiagramButton_clicked();

    void on_unloadCSVButton_clicked(); //Выгрузить в CSV
    void on_unloadHTMLButton_clicked(); //Выгрузить в HTML

signals:
    void mainWindowClosed(); //Закрытие окна

private:
    Ui::MainWindow *ui;

    QString m_operator; //Название выбранного пользователем оператора
    int m_countThread; //Количество потоков
    int m_countNumber; //Количетво изучаемых номеров

    int m_from; //Начальное значение (для перебора по номерам)
    int m_before; //Конечное значение (для перебора по номерам)

    int m_countAnotherOperator; //Количество других операторов
    int m_countChooseOperator; //Количество исследуемых операторов (из отработанных номеров)
    int m_countProcessedNumber; //Количество обработанных номеров
    int m_countFailedNumber; //Количество необработанных (ошибка 500 сервера) номеров

    QTableWidgetItem *m_item; //Элемент таблицы (ячейка)

    int m_tableColumn; //Номер строки в таблице (для вывода на экран)

    int m_countOther; //Количество несовпадений с другими операторами

    QList<QString> m_otherOperators; //Список других операторов

    FileHandler m_fl; //Для загрузки стилей приложения

    QString m_operatorCode; //Код оператора

    QList<int> m_countOtherOperators; //Количество других операторов

    QThread *thread[];
    Worker *w[];

    GraphicWindow *m_gw; //Форма отображения графика

    void getOtherOperators(); //Получить список с количеством других операторов

    void getResearch(); //Провести исследование по номерам для отправки пользователю отчета
    QByteArray m_diargamData; //Диаграмма

    QString m_rangeNumbers; //Диапазон исследуемых номеров
    QString m_operatorName; //Название исследуемого оператора

    QByteArray m_tableResult; //Таблица с номерами

    QMovie *m_mov; //Прелоадер

    int m_currentServerCode;

    int m_mnpServiceCode; //MNP-сервис

    int m_countFactChangeOperator; //Количество фактов смены оператора

    int m_countRangeNumber; //Количество номеров в диапазоне (емкость диапазона)
};

#endif // MAINWINDOW_H
