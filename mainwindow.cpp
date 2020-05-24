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
#include <QCloseEvent>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    settings = new QSettings("l4gfcm", "HRAssistantTool", this);

    if(connectDatabase()){
    ui->setupUi(this);
    initApp();
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

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox confirm(this);
    confirm.setText("Are you sure want to exit?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(confirm.exec() == QMessageBox::Yes){
        saveSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

bool MainWindow::connectDatabase(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    Login loginDialog;
    settings->beginGroup("login");
    loginDialog.setHost(settings->value("host", "").toString());
    loginDialog.setDbName(settings->value("dbName", "").toString());
    loginDialog.setLogin(settings->value("userName", "").toString());
    settings->endGroup();

    bool isDbValid = true;

    do{
        isDbValid = true;
        if (loginDialog.exec() == QDialog::Accepted){
            db.setHostName(loginDialog.getHost());
            db.setDatabaseName(loginDialog.getDbName());
            db.setUserName(loginDialog.getLogin());
            db.setPassword(loginDialog.getPassword());
            if (!db.open())
                QMessageBox::critical(&loginDialog, "Error!", "Database is not conected.");
            else if (!DBHandler::validateDatabase(db)) {
                QMessageBox::critical(&loginDialog, "Error!", "Connected database is invalid.");
                isDbValid = false;
            }
        }
        else{
            return false;
        }

    } while (!db.isOpen() || !isDbValid);
    isValid = true;
    saveLogin(loginDialog.getHost(),loginDialog.getDbName(), loginDialog.getLogin());
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

    mainBarActions.push_back(new QAction("Edit Record", this));
    ui->toolBar->addAction(mainBarActions[at(Actions::EditRecord)]);
    connect(mainBarActions[at(Actions::EditRecord)], &QAction::triggered, this,
            [this](){this->editRecord(ui->table->currentIndex());});
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

    initMainBar();
    loadSettings();
    ui->statusBar->showMessage("Database is connected successfully.", 3000);
}

void MainWindow::addWorker(){
    bool getStatus[2] = {};
    Vacancies vacancies = dbHandler->getVacancies(&getStatus[0]);
    auto maxCommentSize =dbHandler->getMaxCommentSize(&getStatus[1]);

    if(!(getStatus[0] && getStatus[1])){
        printError(ErrorType::GetData);
        return;
    }

    std::sort(vacancies.begin(), vacancies.end(), [](const auto a, const auto b){
        return a.second < b.second;}
    );

    QStringList vacList;
    for (auto vacancy : vacancies) {
        vacList.push_back(vacancy.second);
    }


    NewWorker dialogNW(this);
    dialogNW.setVacancies(vacList);
    dialogNW.setMaxCommentSize(maxCommentSize);

    if(dialogNW.exec() == QDialog::Accepted){
        const bool status = dbHandler->addWorker(
                    std::make_pair(dialogNW.getFirsName(), dialogNW.getLastName()),
                    dialogNW.getPhoneNumber(),
                    dialogNW.getNextDateTime(),
                    vacancies[dialogNW.getVacancyIndex()].first,
                    dialogNW.getComment()
                    );

        if(status){
            ui->statusBar->showMessage("Worker added successfully.", 3000);
            table->select();
        }
        else
            printError(ErrorType::DoOperation);
    }
}

void MainWindow::editRecord(const QModelIndex &index){
    bool getStatus[3] = {};
    WorkFlow workerWorkFlow = dbHandler->getWorkerWorkflow(index.siblingAtColumn(0).data().toUInt(), &getStatus[0]);
    History workerHistory = dbHandler->getWorkerHistory(index.siblingAtColumn(0).data().toUInt(), &getStatus[1]);
    auto maxCommentSize = dbHandler->getMaxCommentSize(&getStatus[2]);

    if(!(getStatus[0] && getStatus[1] && getStatus[2])){
        printError(ErrorType::GetData);
        return;
    }

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
    editDialog.setHistory(workerHistory);
    editDialog.setMaxCommentSize(maxCommentSize);

    if(editDialog.exec() == QDialog::Accepted){
        const bool status = dbHandler->updateWorker(
                    index.siblingAtColumn(0).data().toUInt(),
                    workerWorkFlow[editDialog.getStateId()].first,
                    editDialog.getComment(),
                    editDialog.getNextDate()
                );

        if(status){
            ui->statusBar->showMessage("Record edited successfully.", 3000);
            table->select();
        }
        else
            printError(ErrorType::DoOperation);
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
        bool status = dbHandler->deleteWorker(ui->table->currentIndex().siblingAtColumn(0).data().toUInt());
        if(status){
            ui->statusBar->showMessage("Worker is deleted successfully.", 3000);
            table->select();
        }
        else
            printError(ErrorType::DoOperation);
    }
}

void MainWindow::manageVacancies(){
    bool getStatus = true;
    ManageVacancies manageVacDialog;
    Vacancies vacancies = dbHandler->getVacancies(&getStatus);
    QStringList vacList;

    if(getStatus == false){
        printError(ErrorType::GetData);
        return;
    }

    std::sort(vacancies.begin(), vacancies.end(), [](const auto a, const auto b){
        return a.second < b.second;}
    );

    for (const auto &vac : vacancies) {
        vacList.push_back(vac.second);
    }

    manageVacDialog.setVacancies(vacList);
    manageVacDialog.exec();
    bool status = true;

    switch (manageVacDialog.command) {
    case ManageVacancies::CommandType::None:
        break;
    case ManageVacancies::CommandType::Add:{
        status = dbHandler->addVacancy(manageVacDialog.getVacancyName());
        break;
    }
    case ManageVacancies::CommandType::Delete:{
        auto deleteList = manageVacDialog.getSelectedItems();
        std::list<uint16_t> vacKeys;
        for (const auto &vacancy : deleteList) {
            vacKeys.push_back(vacancies[vacancy].first);
        }
        status = dbHandler->deleteVacancies(vacKeys);
        break;
    }
    }
    if(status){
        ui->statusBar->showMessage("Operation completed successfully.", 3000);
        table->select();
    }
    else
        printError(ErrorType::DoOperation);
}

void MainWindow::restartWorkflow(){
    bool getStatus[2] = {};
    const auto workerPK = ui->table->currentIndex().siblingAtColumn(0).data().toUInt();

    const auto workerVacancyKey = dbHandler->getWorkerVacancy(workerPK, &getStatus[0]);
    Vacancies vacansies = dbHandler->getVacancies(&getStatus[1]);

    if(!(getStatus[0] && getStatus[0])){
        printError(ErrorType::GetData);
        return;
    }

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
        const bool status = dbHandler->restartWorkflow(
                    ui->table->currentIndex().siblingAtColumn(0).data().toUInt(),
                    vacansies[restartDilog.getVacancy()].first,
                    restartDilog.getNextDate()
                    );
        if(status){
            ui->statusBar->showMessage("Workflow restarted successfully.", 3000);
            table->select();
        }
        else
            printError(ErrorType::DoOperation);
    }
}

bool MainWindow::isFailed(){
    return !isValid;
}

void MainWindow::loadSettings(){
    settings->beginGroup("mainWindow");
    resize(settings->value("size", this->size()).toSize());
    move(settings->value("position", this->pos()).toPoint());
    addToolBar(static_cast<Qt::ToolBarArea>(
                   settings->value("toolBar", Qt::ToolBarArea::TopToolBarArea).toInt()),
                   ui->toolBar);

    settings->endGroup();

    settings->beginGroup("table");
    settings->beginReadArray("columnWidths");
    const auto colunmCount = ui->table->horizontalHeader()->count() - 1; //first column is invisible
    for (auto i = 0; i < colunmCount; i++ ) {
        settings->setArrayIndex(i);
        ui->table->resizeColumnToContents(i + 1);
        if(ui->table->columnWidth(i + 1) < settings->value("columnWidth", 0).toInt())
            ui->table->setColumnWidth(i + 1, settings->value("columnWidth").toInt());
    }
    settings->endArray();
    ui->table->horizontalHeader()->setSortIndicator(
                settings->value("sortIndex", 1).toInt(),
                static_cast<Qt::SortOrder>(settings->value("sortOrder", 0).toInt())
                );
    settings->endGroup();
}

void MainWindow::saveSettings(){

    settings->beginGroup("mainWindow");
    settings->setValue("size", this->size());
    settings->setValue("positon", this->pos());
    settings->setValue("toolBar", this->toolBarArea(ui->toolBar));
    settings->endGroup();

    settings->beginGroup("table");
    const auto colunmCount = ui->table->horizontalHeader()->count() - 1; //first column is invisible
    settings->beginWriteArray("columnWidths", colunmCount);

    for (auto i = 0; i < colunmCount; i++) {
        settings->setArrayIndex(i);
        settings->setValue("columnWidth", ui->table->columnWidth(i));
    }
    settings->endArray();
    settings->setValue("sortIndex", ui->table->horizontalHeader()->sortIndicatorSection());
    settings->setValue("sortOrder", ui->table->horizontalHeader()->sortIndicatorOrder());
    settings->endGroup();

}

void MainWindow::saveLogin(const QString &hostName, const QString &dbName,
                           const QString &userName){
    settings->beginGroup("login");
    settings->setValue("host", hostName);
    settings->setValue("dbName", dbName);
    settings->setValue("userName", userName);
    settings->endGroup();
}

void MainWindow::printError(const ErrorType &&val){
    switch (val){
    case ErrorType::GetData:
        ui->statusBar->showMessage("Failed! Unable to load data. Please restart the software.", 5000);
        break;
    case ErrorType::DoOperation:
        ui->statusBar->showMessage("Failed! Something has gone wrong. Please restart the software.", 5000);
        break;
    }
}
