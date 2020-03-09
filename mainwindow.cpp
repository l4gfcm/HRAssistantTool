#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "newworker.h"
#include "editrecord.h"
#include "filter.h"
#include "managevacancies.h"
#include "restartworkflow.h"
#include "login.h"

#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QDial>
#include <QDateTime>
#include <QDebug>
#include <QSqlIndex>
#include <vector>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    if(connectDatabase()){
    ui->setupUi(this);
    initApp();
    initMainBar();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete filterModel;
    delete table;
    delete dbHandler;
    for (auto &item  : mainBarActions) {
        delete item;
    }
}

bool MainWindow::connectDatabase(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    Login loginDialog;

    do{
        if (loginDialog.exec() == QDialog::Accepted){
            db.setHostName(loginDialog.getHost());
            db.setDatabaseName(loginDialog.getDbName());
            db.setUserName(loginDialog.getLogin());
            db.setPassword(loginDialog.getPassword());
            if (db.open())
                QMessageBox::information(&loginDialog, "OK", "Database is connected.");
            else
                QMessageBox::critical(&loginDialog, "Error!", "Database is not conected!");
        }
        else{
            return false;
        }

    } while (!db.isOpen());
    return true;

}

void MainWindow::initMainBar(){
    mainBarActions.push_back(new QAction("Add worker", this));
    ui->toolBar->addAction(mainBarActions[at(Actions::AddWorker)]);
    connect(mainBarActions[at(Actions::AddWorker)], &QAction::triggered, this, &MainWindow::addWorker);

    mainBarActions.push_back(new QAction("Delete worker", this));
    ui->toolBar->addAction(mainBarActions[at(Actions::DeleteWorker)]);
    connect(mainBarActions[at(Actions::DeleteWorker)], &QAction::triggered, this, &MainWindow::deleteWorker);

    mainBarActions.push_back(new QAction("Manage Vacancies", this));
    ui->toolBar->addAction(mainBarActions[at(Actions::ManageVacancies)]);
    connect(mainBarActions[at(Actions::ManageVacancies)], &QAction::triggered, this, &MainWindow::manageVacancies);

    mainBarActions.push_back(new QAction("Restart Workflow", this));
    ui->toolBar->addAction(mainBarActions[at(Actions::RestartWorkflow)]);
    connect(mainBarActions[at(Actions::RestartWorkflow)], &QAction::triggered, this, &MainWindow::restartWorkflow);
}

void MainWindow::initApp(){
    dbHandler = new DBHandler(db);
    table = new QSqlRelationalTableModel(this, db);
    table->setTable("workers");

    table->setHeaderData(1, Qt::Horizontal, "First name");
    table->setHeaderData(2, Qt::Horizontal, "Last name");
    table->setHeaderData(3, Qt::Horizontal, "Phone");
    table->setHeaderData(4, Qt::Horizontal, "Next date");
    table->setHeaderData(5, Qt::Horizontal, "Vacancy");
    table->setHeaderData(6, Qt::Horizontal, "State");
    table->setRelation(5, QSqlRelation("vacancies", "id_vacancy", "vname"));
    table->setRelation(6, QSqlRelation("workflow", "id_step", "sname"));

    table->select();

    filterModel = new Filter(this);
    filterModel->setSourceModel(table);
    connect(ui->filterLine, &QLineEdit::textChanged, filterModel, &Filter::filterTextChanged);

    ui->table->setModel(filterModel);
    ui->table->setEditTriggers(QTableView::NoEditTriggers);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setColumnHidden(0, true);
    ui->table->setSortingEnabled(true);

    connect(ui->table, &QTableView::doubleClicked,
          this, &MainWindow::editRecord);
}

void MainWindow::addWorker(){
    Vacancies vacancies = dbHandler->getVacancies();
    QStringList vacList;
    for (auto vacancy : vacancies) {
        vacList.push_back(vacancy.second);
        qDebug() << vacancy;
    }

    NewWorker dialogNW(this);
    dialogNW.setVacancies(vacList);

    if(dialogNW.exec() == QDialog::Accepted){
        dbHandler->addWorker(
                    std::make_pair(dialogNW.getFirsName(), dialogNW.getLastName()),
                    dialogNW.getPhoneNumber(),
                    dialogNW.getNextDateTime(),
                    vacancies[dialogNW.getVacancyIndex()].first,
                    dialogNW.getComment()
                    );

        table->select();
    }
}

void MainWindow::editRecord(const QModelIndex &index){

    WorkFlow workerWorkFlow = dbHandler->getWorkerWorkflow(index.siblingAtColumn(0).data().toUInt());
    QStringList stepList;
    for (const auto &step : workerWorkFlow) {
        stepList.push_back(step.second);
    }

    WorkerName name = std::make_pair(
                ui->table->currentIndex().siblingAtColumn(1).data().toString(),
                ui->table->currentIndex().siblingAtColumn(2).data().toString()
                );


    EditRecord editDialog(this);
    editDialog.setName(name);
    editDialog.setSteps(stepList);
    editDialog.setHistory(dbHandler->getWorkerHistory(index.siblingAtColumn(0).data().toUInt()));

    if(editDialog.exec() == QDialog::Accepted){
        dbHandler->updateWorker(index.siblingAtColumn(0).data().toUInt(),
                                 workerWorkFlow[editDialog.getStateId()].first, editDialog.getComment());
        table->select();

    }

}

void MainWindow::deleteWorker(){
    WorkerName name = std::make_pair(
                ui->table->currentIndex().siblingAtColumn(1).data().toString(),
                ui->table->currentIndex().siblingAtColumn(2).data().toString()
                );

    QMessageBox confirm(this);
    confirm.setText(QString("Are you sure want to delete ")
                    .append(name.first).append(" ").append(name.second).append("?"));
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if(confirm.exec() == QMessageBox::Yes){
        dbHandler->deleteWorker(ui->table->currentIndex().siblingAtColumn(0).data().toUInt());
        table->select();
    }
}

void MainWindow::manageVacancies(){
    ManageVacancies manageVacDialog;
    Vacancies vacancies = dbHandler->getVacancies();
    QStringList vacList;

    for (const auto &vac : vacancies) {
        vacList.push_back(vac.second);
    }

    manageVacDialog.setVacancies(vacList);
    manageVacDialog.exec();
    switch (manageVacDialog.command) {
    case ManageVacancies::CommandType::None:
        break;
    case ManageVacancies::CommandType::Add:{
        dbHandler->addVacancy(manageVacDialog.getVacancyName());
        break;
    }
    case ManageVacancies::CommandType::Delete:{
        auto deleteList = manageVacDialog.getSelectedItems();
        std::list<uint16_t> vacKeys;
        for (const auto &vacancy : deleteList) {
            vacKeys.push_back(vacancies[vacancy].first);
        }
        dbHandler->deleteVacancies(vacKeys);
        break;
    }
    }
    table->select();
}

void MainWindow::restartWorkflow(){
    const auto workerPK = ui->table->currentIndex().siblingAtColumn(0).data().toUInt();
    const auto workerVacancyKey = dbHandler->getWorkerVacancy(workerPK);

    Vacancies vacansies = dbHandler->getVacancies();
    QStringList vacanciesNames;
    uint16_t currentVacancyIndex = 0;

    for(size_t i  = 0; i < vacansies.size(); i++){
        vacanciesNames.push_back(vacansies[i].second);
        if(workerVacancyKey == vacansies[i].first){
            currentVacancyIndex = i;
        }
    }

    RestartWorkflow restartDilog(this);
    restartDilog.setVacancies(vacanciesNames,
                              currentVacancyIndex);
    restartDilog.setWorkerName(std::make_pair(
                ui->table->currentIndex().siblingAtColumn(1).data().toString(),
                ui->table->currentIndex().siblingAtColumn(2).data().toString())
                );
    if(restartDilog.exec() == QDialog::Accepted){
        dbHandler->restartWorkflow(
                    ui->table->currentIndex().siblingAtColumn(0).data().toUInt(),
                    vacansies[restartDilog.getVacancy()].first,
                    restartDilog.getNextDate()
                    );
        table->select();
    }
}

bool MainWindow::isFailed(){
    return !db.isOpen();
}
