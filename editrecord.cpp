#include "editrecord.h"
#include "ui_editrecord.h"
#include <QRadioButton>
#include <QDebug>
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

void EditRecord::setSteps(const std::vector<std::pair<uint16_t, QString> > &steps){
    qDebug() << steps.size();
    for(size_t i = 0; i < steps.size(); i++){
        auto button = new QGroupBox(this);
        button->setTitle(steps[i].second);
        ui->WorkflowButtons->addWidget(button);
    }

}
