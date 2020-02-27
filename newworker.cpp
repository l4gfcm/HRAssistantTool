#include "newworker.h"
#include "ui_newworker.h"

NewWorker::NewWorker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWorker)
{
    ui->setupUi(this);
}

NewWorker::~NewWorker()
{
    delete ui;
}

void NewWorker::setVacancies(const QStringList &vac){
    ui->vacancyDD->addItems(vac);
}
QString NewWorker::getComment(){
    return ui->commentEdit->toPlainText();
}

QString NewWorker::getFirsName(){
    return ui->fnameLine->text();
}

QString NewWorker::getLastName(){
    return ui->lnameLine->text();
}

QString NewWorker::getPhoneNumber(){
    return ui->phoneLine->text();
}

uint16_t NewWorker::getVacancyIndex(){
    return ui->vacancyDD->currentIndex();
}

QDateTime NewWorker::getNextDateTime(){
    return ui->dateTime->dateTime();
}
