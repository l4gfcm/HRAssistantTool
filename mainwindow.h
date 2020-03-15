#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSettings>

#include "dbhandler.h"

class QSqlRelationalTableModel;
class QSqlDatabase;
class Filter;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isFailed();
private slots:
    void addWorker();
    void editRecord(const QModelIndex &index);
    void deleteWorker();
    void manageVacancies();
    void restartWorkflow();
private:
    QSqlDatabase db;
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *table;
    Filter *filterModel;
    DBHandler *dbHandler;
    std::vector<QAction*> mainBarActions;
    QSettings *settings;
    bool connectDatabase();
    void initApp();
    void initMainBar();
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);
    void saveLogin(const QString &hostName,
                   const QString &dbName, const QString &userName);
    enum class Actions : uint8_t{
        AddWorker,
        DeleteWorker,
        ManageVacancies,
        RestartWorkflow,
        EditRecord
    };
    constexpr size_t at(const Actions value) const{
        return static_cast<size_t>(value);
    }
};
#endif // MAINWINDOW_H
