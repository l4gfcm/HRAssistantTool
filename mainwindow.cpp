#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "newworker.h"
#include "editrecord.h"

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
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(! connectDatabase())
        qApp->quit();
    initApp();

    addWorkerAction = new QAction("Add worker",this);
    ui->toolBar->addAction(addWorkerAction);
    connect(addWorkerAction, &QAction::triggered, this, &MainWindow::addWorker);

    connect(ui->table, &QTableView::doubleClicked,
          this, &MainWindow::editRecord);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::connectDatabase(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("hrat");
    db.setUserName("root");
    db.setPassword("123456");
    if (db.open()){
        QMessageBox::information(this, "OK", "Database is connected");
        return true;}
    else{
        QMessageBox::critical(this, "Error!", "Database is not conected!");
        return false;
    }
}

void MainWindow::initApp(){
    table = new QSqlRelationalTableModel(nullptr, db);
        table->setTable("workers");

        table->setHeaderData(1, Qt::Horizontal, "First name");
        table->setHeaderData(2, Qt::Horizontal, "Last name");
        table->setHeaderData(3, Qt::Horizontal, "Phone");
        table->setHeaderData(4, Qt::Horizontal, "Next date");
        table->setHeaderData(5, Qt::Horizontal, "Vacancy");
        table->setHeaderData(6, Qt::Horizontal, "State");
        table->setRelation(5, QSqlRelation("vacancies", "id_vacancy", "vname"));
        table->setRelation(6, QSqlRelation("workflow", "id_step", "name"));

        table->select();

        ui->table->setModel(table);
        ui->table->setEditTriggers(QTableView::NoEditTriggers);
        ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table->setColumnHidden(0, true);
        ui->table->setSortingEnabled(true);
}
bool MainWindow::saveToHistory(const int32_t &user, const int32_t &step, const QString &comment){
    QSqlQuery query("INSERT INTO history (id_history, fid_worker, comment, fid_step, date) "
                        "VALUES (?, ?, ?, ?, ?)");
    query.bindValue(1, user);
    query.bindValue(2, comment);
    query.bindValue(3, step);
    query.bindValue(4, QDateTime::currentDateTime());
    return query.exec();
}

void MainWindow::addWorker(){
    QStringList vacancies;
    std::vector<uint16_t> indexToID;
    QSqlQuery getVacancy("SELECT * FROM vacancies");
    int fieldNo = getVacancy.record().indexOf("vname");
    for (uint16_t index = 0; getVacancy.next(); index++) {
        QString vacancy = getVacancy.value(fieldNo).toString();
        vacancies.push_back(vacancy);
        indexToID[index] = getVacancy.record().value(0).toInt();
        qDebug() << indexToID[index];
    }

    NewWorker dialogNW(this);
    dialogNW.setVacancies(vacancies);
    dialogNW.exec();

    QSqlQuery insertToWorkers;
    insertToWorkers.prepare("INSERT INTO workers (id_worker, fname, lname, mphone, next_date, fd_vacancy, fd_state) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?)");
     //AutoIncrement
    insertToWorkers.bindValue(1, dialogNW.getFirsName());
    insertToWorkers.bindValue(2, dialogNW.getLastName());
    insertToWorkers.bindValue(3, dialogNW.getPhoneNumber());
    insertToWorkers.bindValue(4, dialogNW.getNextDateTime());
    insertToWorkers.bindValue(5, indexToID[dialogNW.getVacancyIndex()]);
    insertToWorkers.bindValue(6, 0); // New records get a start state
    insertToWorkers.exec();

    saveToHistory(insertToWorkers.lastInsertId().toInt(), 0, dialogNW.getComment());

    table->select();
}

void MainWindow::editRecord(const QModelIndex &index){

EditRecord().exec();

}
