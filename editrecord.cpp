#include "editrecord.h"
#include "ui_editrecord.h"

#include <QRadioButton>
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>


EditRecord::EditRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditRecord)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

EditRecord::~EditRecord()
{
    delete ui;
}

void EditRecord::setFLName(const QString &flname){
    setWindowTitle(flname);
}

void EditRecord::setSteps(const std::vector<QString> &steps){
    buttonsGroup = new QButtonGroup(this);
    connect(buttonsGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &EditRecord::checkInput);
    for(size_t i = 0; i < steps.size(); i++){
        auto button = new QRadioButton(this);
        button->setText(steps[i]);
        buttonsGroup->addButton(button, i);
        ui->WorkflowButtons->addWidget(button);
    }
}

QString EditRecord::getComment(){
    return ui->commentEdit->toPlainText();
}

QDateTime EditRecord::getNextDate(){
    return ui->nextDateTime->dateTime();
}

uint16_t EditRecord::getStateId(){
    return buttonsGroup->checkedId(); // -1 blocked by GUI
}


void EditRecord::checkInput(){
    if (buttonsGroup->checkedId() == -1){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void EditRecord::setHistory(const std::vector<std::tuple<QString, QString, QDateTime> > & history){

    for (size_t i = 0, row = 0 ; i < history.size() ; i++, row = row + 2) {
        auto step = new QLabel(std::get<0>(history[i]), this);
        auto comment = new QLabel(std::get<1>(history[i]), this);
        auto date = new QLabel(std::get<2>(history[i]).toString(), this);

        ui->gridLayoutHistrory->addWidget(step, row, 0, Qt::AlignTop);
        ui->gridLayoutHistrory->addWidget(date, row, 1, Qt::AlignTop);
        ui->gridLayoutHistrory->addWidget(comment, row + 1, 0, Qt::AlignTop);

    }
}

