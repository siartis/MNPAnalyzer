#include "graphicwindow.h"
#include "ui_graphicwindow.h"

GraphicWindow::GraphicWindow(const QStringList &otherOperators,
                             const QStringList &countOtherOperators,
                             const int countNumber,
                             QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphicWindow)
{
    ui->setupUi(this);

    m_otherOperators = otherOperators;
    m_countOtherOperators = countOtherOperators;
    m_countNumber = countNumber;

    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Оператор"));
    model->setHeaderData(1, Qt::Horizontal, tr("Количество номеров"));

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    QTableView *table = new QTableView;
    pieChart = new PieView;
    splitter->addWidget(table);
    splitter->addWidget(pieChart);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(0, 0);

    table->setStyleSheet("background: white; ");

    table->setModel(model);
    pieChart->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setStretchLastSection(true);

    setCentralWidget(splitter);

    QStringList colors;
    colors << "#FFFF00" << "#660000" << "#000033" << "#00FF00" << "#999999";
    colors << "#009999" << "#6699FF" << "#660066" << "#330000" << "#CC6600";

    int row = 0; //Количество строк таблицы
    for(int i = 0; i < m_otherOperators.length(); i++) {
        model->insertRows(row, 1, QModelIndex());
        model->setData(model->index(row, 0, QModelIndex()),
                       QString("%1 - %2 (%3%)")
                       .arg(m_otherOperators.at(i))
                       .arg(m_countOtherOperators.at(i))
                       .arg((m_countOtherOperators.at(i).toInt()*100)/m_countNumber)); //Название оператора

        model->setData(model->index(row, 1, QModelIndex()),
                       QString("%1").arg(m_countOtherOperators.at(i).toInt())); //Количество номеров с другим оператором

        model->setData(model->index(row, 0, QModelIndex()),
                       QColor(colors.at(i)), Qt::DecorationRole); //Цвет
    }

    //Запрет изменения ячеек
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

GraphicWindow::~GraphicWindow()
{
    delete ui;
}

void GraphicWindow::on_action_triggered()
{
    QPixmap p;
    p = QPixmap::grabWidget(pieChart, 5, 5, 600, 500);

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");

    emit getReport(bArray);
    bArray.clear();
}

//Закрытие окна
void GraphicWindow::closeEvent(QCloseEvent *e)
{
    this->close();
    emit graphicWindowClosed();
}

//Вернуться назад
void GraphicWindow::on_action_3_triggered()
{
    this->close();
    emit graphicWindowClosed();
}

//Выход
void GraphicWindow::on_action_2_triggered()
{
    qApp->quit();
}
