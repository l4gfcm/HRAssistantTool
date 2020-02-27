#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

class QSqlRelationalTableModel;
class QSqlDatabase;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase db;
private slots:
    void addWorker();
    void editRecord(const QModelIndex &index);
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *table;
    bool connectDatabase();
    void initApp();
    bool saveToHistory(const int32_t &user, const int32_t &step, const QString &comment);

    QAction *addWorkerAction;
};
#endif // MAINWINDOW_H
