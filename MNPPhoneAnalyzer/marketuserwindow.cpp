#include "marketuserwindow.h"
#include "ui_marketuserwindow.h"

MarketUserWindow::MarketUserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarketUserWindow)
{
    ui->setupUi(this);

    //Прогрузка стилей приложения
    this->setStyleSheet("background-image:url(\":/style/fon.png\")");

    //Вывод на экран списка регионов РФ
    QStringList regions;
    regions.clear();

    //regions << "Республика Адыгея"<< "Республика Башкортостан"<< "Республика Бурятия"<< "Республика Алтай"<< "Республика Дагестан"<< "Республика Ингушетия"<< "Кабардино-Балкарская Республика"<< "Республика Калмыкия"<< "Республика Карачаево-Черкесия"<< "Республика Карелия"<< "Республика Коми"<< "Республика Марий Эл"<< "Республика Мордовия"<< "Республика Саха (Якутия)"<< "Республика Северная Осетия-Алания"<< "Республика Татарстан"<< "Республика Тыва"<< "Удмуртская Республика"<< "Республика Хакасия"<< "Чеченская республика"<< "Чувашская Республика"<< "Алтайский край" << "Краснодарский край"<< "Красноярский край" << "Приморский край" << "Ставропольский край" << "Хабаровский край" << "Амурская область" << "Архангельская область" << "Астраханская область" << "Белгородская область" << "Брянская область"<< "Владимирская область"<< "Волгоградская область" << "Вологодская область" << "Воронежская область" << "Ивановская область" << "Иркутская область"<< "Калининградская область"<< "Калужская область"<< "Камчатский край"<< "Камчатская область"<< "Кемеровская область"<< "Кировская область"<< "Костромская область"<< "Курганская область"<< "Курская область"<< "Ленинградская область"<< "Липецкая область"<< "Магаданская область"<< "Московская область"<< "Мурманская область"<< "Нижегородская область"<< "Новгородская область"<< "Новосибирская область"<< "Омская область"<< "Оренбургская область"<< "Орловская область"<< "Пензенская область"<< "Пермский край"<< "Пермская область"<< "Псковская область"<< "Ростовская область"<< "Рязанская область"<< "Самарская область"<< "Саратовская область"<< "Сахалинская область"<< "Свердловская область"<< "Смоленская область"<< "Тамбовская область"<< "Тверская область"<< "Томская область"<< "Тульская область"<< "Тюменская область"<< "Ульяновская область"<< "Челябинская область"<< "Забайкальский край"<< "Читинская область"<< "Ярославская область"<< "г. Москва"<< "г. Санкт-Петербург"<< "Еврейская автономная область"<< "Агинский Бурятский автономный округ"<< "Коми-Пермяцкий автономный округ"<< "Корякский автономный округ"<< "Ненецкий автономный округ"<< "Таймырский (Долгано-Ненецкий) автономный округ"<< "Усть-Ордынский Бурятский автономный округ"<< "Ханты-Мансийский автономный округ - Югра"<< "Чукотский автономный округ"<< "Эвенкийский автономный округ"<< "Ямало-Ненецкий автономный округ"<< "Республика Крым"<< "г.Севастополь"<< "Чеченская Республика";
    regions.append("Московская область");
    regions.append("Самарская область");

    ui->regionBox->addItems(regions);
    regions.clear();

    ui->regionBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
    ui->operatorBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));
    ui->codeOperatorBox->setStyleSheet(m_fl.getStyle(":/style/comboStyle.css"));

    ui->nextButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));
    ui->quitButton->setStyleSheet(m_fl.getStyle(":/style/buttonStyle.css"));

    ui->chooseCodeOperatorLabel->setVisible(false);
    ui->codeOperatorBox->setVisible(false);
}

MarketUserWindow::~MarketUserWindow()
{
    delete ui;
}

//Закрытие окна
void MarketUserWindow::closeEvent(QCloseEvent *e)
{
    this->close();
    emit marketUserWindowClosed();
}

void MarketUserWindow::on_quitButton_clicked()
{
    qApp->quit();
}

void MarketUserWindow::on_operatorBox_currentIndexChanged(int index)
{
    //Если выбран Билайн
    if (index == 1) {
        ui->chooseCodeOperatorLabel->setVisible(true);
        ui->codeOperatorBox->setVisible(true);
    }

    this->showMaximized();
}

void MarketUserWindow::on_nextButton_clicked()
{
    MarketAnalysisWindow *maw = new MarketAnalysisWindow(ui->regionBox->currentText(),
                                                         this);
    connect(maw, SIGNAL(marketAnalysisWindowClosed()), this, SLOT(showMaximized()));
    maw->showMaximized();
    this->hide();
}
