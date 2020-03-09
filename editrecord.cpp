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
    ui->nextDateTime->setDate(QDate::currentDate());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->commentEdit, &QPlainTextEdit::textChanged, this, &EditRecord::checkCommentInput);
}

EditRecord::~EditRecord()
{
    delete ui;
}

void EditRecord::setName(const std::pair<QString, QString> & name){
    setWindowTitle(QString(name.first).append(" ").append(name.second));
}

void EditRecord::setSteps(const QStringList &steps){
    if(steps.size() == 0){
        ui->tabWidget->widget(0)->setDisabled(true);
        auto info = new QLabel("Restart the worker's workflow to edit.", this);
        info->setAlignment(Qt::AlignCenter);
        ui->WorkflowButtons->addWidget(info);
    }
    buttonsGroup = new QButtonGroup(this);
    connect(buttonsGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &EditRecord::checkInput);
    uint16_t index = 0;
    for(const auto& step : steps ){
        auto button = new QRadioButton(this);
        button->setText(step);
        buttonsGroup->addButton(button, index);
        ui->WorkflowButtons->addWidget(button);
        ++index;
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

void EditRecord::setHistory(const std::vector<std::tuple<QString, QString, QDateTime>> & history){
    for (const auto & record : history) {
        ui->historyTextBrower->append(std::get<0>(record));
        ui->historyTextBrower->setAlignment(Qt::AlignLeft);

        ui->historyTextBrower->append(std::get<2>(record).toString());
        ui->historyTextBrower->setAlignment(Qt::AlignRight);

        if(!std::get<1>(record).isEmpty()){
            ui->historyTextBrower->append(std::get<1>(record));
            ui->historyTextBrower->setAlignment(Qt::AlignLeft);
        }
    }
}

void EditRecord::checkCommentInput(){
    if(ui->commentEdit->toPlainText().size() > 128)
        ui->commentEdit->setPlainText(
                    ui->commentEdit->toPlainText().left(128)
                    );
}


