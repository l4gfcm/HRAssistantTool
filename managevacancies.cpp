#include "managevacancies.h"
#include "ui_managevacancies.h"
#include <QMessageBox>


ManageVacancies::ManageVacancies(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageVacancies)
{
    ui->setupUi(this);
    ui->addVacancy->setDisabled(true);
    ui->deleteVacancy->setDisabled(true);
    connect(ui->addVacancy, &QAbstractButton::clicked, this, &ManageVacancies::addVacancy);
    connect(ui->deleteVacancy, &QAbstractButton::clicked, this, &ManageVacancies::deleteVacancy);
    connect(ui->newVacancyName, &QLineEdit::textChanged, this, &ManageVacancies::checkAddInput);
}

ManageVacancies::~ManageVacancies()
{
    delete ui;
}

void ManageVacancies::setVacancies(const QStringList &vacancies){
    for(const auto &vac : vacancies){
       vacList.push_back(new QCheckBox(vac, this));
       ui->vacanciesList->addWidget(vacList.back());
       connect(vacList.back(), &QCheckBox::toggled, this, &ManageVacancies::checkDeleteInput);
    }
}

std::list<uint16_t> ManageVacancies::getSelectedItems(){
    std::list<uint16_t> result;
    for (size_t i = 0; i < vacList.size(); i++) {
        if(vacList[i]->isChecked())
            result.push_back(i);
    }
    return result;
}

void ManageVacancies::addVacancy(){
    bool isExist = false;
    for (const auto & item : vacList) {
        if (item->text() == ui->newVacancyName->text())
            isExist = true;
    }
    if (isExist){
        QMessageBox(QMessageBox::Warning, "Add vacancy", "Entered vacancy is exist!",
                    QMessageBox::Ok, this).exec();
    }
    else if(
    QMessageBox(QMessageBox::Question, "Add vacancy", "Are you sure want to add the vacancy?",
                QMessageBox::Ok | QMessageBox::Cancel, this).exec() == QMessageBox::Ok){
        command = CommandType::Add;
        this->close();
    }
}
void ManageVacancies::deleteVacancy(){
    if(
    QMessageBox(QMessageBox::Warning, "Delete vacancies", "Are you sure want to delete selected vacancies?\n"
                "It will delete all records that contain the vacancies.",
                QMessageBox::Ok | QMessageBox::Cancel, this).exec() == QMessageBox::Ok){
        command = CommandType::Delete;
        this->close();
    }
}

QString ManageVacancies::getVacancyName(){
    return ui->newVacancyName->text();
}

void ManageVacancies::checkAddInput(){
    if(!ui->newVacancyName->text().isEmpty())
        ui->addVacancy->setEnabled(true);
    else
        ui->addVacancy->setEnabled(false);
}

void ManageVacancies::checkDeleteInput(){
    bool hasSelection = false;
    for (const auto &item : vacList){
        if(item->isChecked()){
            hasSelection = true;
            break;
        }
    }

    if(hasSelection)
        ui->deleteVacancy->setEnabled(true);
    else
        ui->deleteVacancy->setEnabled(false);
}
