#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "newworker.h"
#include "editrecord.h"
#include "filter.h"

#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <QDial>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlIndex>
#include <QSqlRecord>
#include <vector>
#include <utility>
#include <unordered_map>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(! connectDatabase())
        qApp->quit();
    initApp();
    initMainBar();
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
    db.setHostName("localhost");
    db.setDatabaseName("hrat");
    db.setUserName("root");
    db.setPassword("123456");
    if (db.open()){
        QMessageBox::information(this, "OK", "Database is connected");
        dbHandler = new DBHandler(db);
        return true;}
    else{
        QMessageBox::critical(this, "Error!", "Database is not conected!");
        return false;
    }
}

void MainWindow::initMainBar(){
    mainBarActions.push_back(new QAction("Add worker",this));
    ui->toolBar->addAction(mainBarActions[at(Actions::AddWorker)]);
    connect(mainBarActions[at(Actions::AddWorker)], &QAction::triggered, this, &MainWindow::addWorker);
    mainBarActions.push_back(new QAction("Delete worker",this));
    ui->toolBar->addAction(mainBarActions[at(Actions::DeleteWorker)]);
    connect(mainBarActions[at(Actions::DeleteWorker)], &QAction::triggered, this, &MainWindow::deleteWorker);
}

void MainWindow::initApp(){
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

    uint16_t workerPK = index.siblingAtColumn(0).data().toInt();

    QSqlQuery getWorkFlow;
    getWorkFlow.prepare("SELECT `id_step`, `sname` FROM `workflow` WHERE `pid_step` = "
                        "(SELECT fd_state FROM `workers` WHERE `id_worker` = ?)");
    getWorkFlow.bindValue(0, index.siblingAtColumn(0).data().toInt());
    getWorkFlow.exec();

    std::vector<std::pair<uint16_t, QString>> steps;
    std::vector<QString> stepNames;

    while (getWorkFlow.next()) {

        steps.push_back(std::make_pair(getWorkFlow.record().value(0).toInt(), //step id
                                       getWorkFlow.record().value(1).toString())); //step name
        stepNames.push_back(getWorkFlow.record().value(1).toString());
    }

    QSqlQuery getHistory;
    getHistory.prepare("SELECT `workflow`.`sname`, `comment`, `date` FROM `history` "
                       "INNER JOIN `workflow` ON `history`.`fid_step` = `workflow`.`id_step` WHERE fid_worker = ?");
    getHistory.bindValue(0, workerPK);
    getHistory.exec();

    std::vector<std::tuple<QString, QString, QDateTime>> userHistory; //StepName, Comment, date
    while (getHistory.next()) {
        userHistory.push_back(
                    std::make_tuple(
                        getHistory.record().value(0).toString(),
                        getHistory.record().value(1).toString(),
                        getHistory.record().value(2).toDateTime()
                        )
                    );
    }
    EditRecord editDialog(this);
    editDialog.setName(dbHandler->getName(index));
    editDialog.setSteps(stepNames);
    editDialog.setHistory(userHistory);
    qDebug() << workerPK;
    if(editDialog.exec() == QDialog::Accepted){
        QSqlQuery updateStatus;
        updateStatus.prepare("UPDATE `workers` SET `fd_state` = ? WHERE (`id_worker` = ?)");
        updateStatus.bindValue(0, steps[editDialog.getStateId()].first); //step id
        updateStatus.bindValue(1, workerPK); //id value of worker
        updateStatus.exec();
        dbHandler->saveToHistory(workerPK, steps[editDialog.getStateId()].first, editDialog.getComment());
        table->select();

    }

}

void MainWindow::deleteWorker(){
    WorkerName name = dbHandler->getName(ui->table->currentIndex());

    QMessageBox confirm(this);
    confirm.setText(QString("Are you suce want to delete ")
                    .append(name.first).append(" ").append(name.second).append("?"));
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if(confirm.exec() == QMessageBox::Yes){
    QSqlQuery deleteWorker;
    deleteWorker.prepare("DELETE FROM `workers` WHERE (`id_worker` = ?)");
    deleteWorker.bindValue(0, ui->table->currentIndex().siblingAtColumn(0).data().toInt());
    deleteWorker.exec();
    deleteWorker.clear();
    deleteWorker.prepare("DELETE FROM `history` WHERE (`fid_worker` = ?)");
    deleteWorker.bindValue(0, ui->table->currentIndex().siblingAtColumn(0).data().toInt());
    deleteWorker.exec();
    table->select();
    }
}
