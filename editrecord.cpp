#include "editrecord.h"
#include "ui_editrecord.h"

#include <QRadioButton>
#include <QDebug>
//#include;

EditRecord::EditRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditRecord)
{
    ui->setupUi(this);
}

EditRecord::~EditRecord()
{
    delete ui;
    for (auto button : buttons) {
        delete button;
    }
}

void EditRecord::setFLName(const QString &flname){
    setWindowTitle(flname);
}

void EditRecord::setSteps(const std::vector<QString> &steps){
    for(size_t i = 0; i < steps.size(); i++){
        auto button = new QRadioButton(this);
        buttons.push_back(button);
        button->setText(steps[i]);
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
    size_t i = 0;
    while (i < buttons.size()) {
        if(buttons[i]->isChecked()){
            break;
        }
        i++;
    }
    return i;
}
