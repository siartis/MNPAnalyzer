#ifndef MARKETWORKER_H
#define MARKETWORKER_H

#include <QObject>

class marketWorker : public QObject
{
    Q_OBJECT
public:
    explicit marketWorker(const QString &oper, const QString operatorCode, QObject *parent = 0);
    ~marketWorker();

    int from; //Значение от
    int before; //Значение до

private:
    QString m_operator;
    QString m_operatorCode;
    void getOperatorName(const QString &oper); //Получить название выбранного оператора
    void getOperatorCode(const QString &operatorCode); //Получить код оператора
    QString m_Phone; //Номер телефона (без кода оператора)

signals:
    void anotherOperator(); //Другой оператор
    void finish(); //Окончание процесса
    void finishForSort(); //Окончание процесса (для сортировки)
    void numberFinish(QString phoneNumber, QString oper, QString region); //Номер обработан
    void numberError(); //Ошибка отработки номера

public slots:
    void startProcess(); //Начало процесса
};

#endif // MARKETWORKER_H
