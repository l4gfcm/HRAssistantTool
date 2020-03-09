#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::setLogin(const QString &login){
    ui->usernameEdit->setText(login);
}
void Login::setHost(const QString &host){
    ui->serverEdit->setText(host);
}
void Login::setDbName(const QString &dbName){
    ui->dbNameEdit->setText(dbName);
}
QString Login::getLogin(){
    return ui->usernameEdit->text();
}
QString Login::getHost(){
    return ui->serverEdit->text();
}
QString Login::getPassword(){
    return ui->passwordEdit->text();
}
QString Login::getDbName(){
    return ui->dbNameEdit->text();
}

