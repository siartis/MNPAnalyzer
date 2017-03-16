#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QMainWindow>
#include <QStringList>

#include "pieview.h"

#include <QTableView>
#include <QSplitter>
#include <QStandardItemModel>
#include <QFile>
#include <QDesktopWidget>
#include <QBuffer>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAbstractItemView;
class QItemSelectionModel;
QT_END_NAMESPACE

namespace Ui {
class GraphicWindow;
}

class GraphicWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphicWindow(const QStringList &otherOperators,
                           const QStringList &countOtherOperators,
                           const int countNumber,
                           QWidget *parent = 0);
    ~GraphicWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    //Элементы меню
    void on_action_triggered(); //Получить отчет с диаграммой на почту
    void on_action_2_triggered(); //Выход
    void on_action_3_triggered(); //Вернуться назад

signals:
    void getReport(QByteArray diagram); //Получить отчет с диаграммой на почту
    void graphicWindowClosed(); //Закрытие окна

private:
    Ui::GraphicWindow *ui;

    QAbstractItemModel *model;
    QAbstractItemView *pieChart; //Диаграмма
    QItemSelectionModel *selectionModel;

    int m_countNumber; //Общее количество номеров
    QStringList m_otherOperators; //Другие операторы
    QStringList m_countOtherOperators; //Количество других операторов
};

#endif // GRAPHICWINDOW_H
