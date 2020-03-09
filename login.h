#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void setLogin(const QString &login);
    void setHost(const QString &host);
    void setDbName(const QString &dbName);
    QString getLogin();
    QString getHost();
    QString getPassword();
    QString getDbName();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
