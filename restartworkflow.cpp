#include "restartworkflow.h"
#include "ui_restartworkflow.h"

RestartWorkflow::RestartWorkflow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestartWorkflow)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDate(QDate::currentDate());
}

RestartWorkflow::~RestartWorkflow()
{
    delete ui;
}

void RestartWorkflow::setVacancies(const QStringList &vacancies, const uint16_t &currentVacancy){
    ui->vacancyComboBox->addItems(vacancies);
    ui->vacancyComboBox->setCurrentIndex(currentVacancy);
}

uint16_t RestartWorkflow::getVacancy(){
    return ui->vacancyComboBox->currentIndex();
}

QDateTime RestartWorkflow::getNextDate(){
    return ui->dateTimeEdit->dateTime();
}

void RestartWorkflow::setWorkerName(const std::pair<QString, QString> &name){
    setWindowTitle(QString("Restart workflof of ").append(name.first)
                   .append(" ").append(name.second).append("?"));
}
