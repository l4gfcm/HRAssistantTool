#include "newworker.h"
#include "ui_newworker.h"
#include <QPushButton>
#include <QDebug>

NewWorker::NewWorker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWorker)
{
    ui->setupUi(this);
    ui->dateTime->setDate(QDate::currentDate());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->lnameLine, &QLineEdit::editingFinished, this, &NewWorker::checkInput);
    connect(ui->fnameLine, &QLineEdit::editingFinished, this, &NewWorker::checkInput);
    connect(ui->phoneLine, &QLineEdit::editingFinished, this, &NewWorker::checkInput);
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

void NewWorker::checkInput(){
    qDebug() << "sfsfs";
    if(
            ui->fnameLine->text().isEmpty() ||
            ui->lnameLine->text().isEmpty() ||
            ui->phoneLine->text().isEmpty()
            )
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
