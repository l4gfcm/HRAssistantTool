#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

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
    QSqlDatabase db;
private slots:
    void addWorker();
    void editRecord(const QModelIndex &index);
    void deleteWorker();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *table;
    Filter *filterModel;
    DBHandler *dbHandler;
    bool connectDatabase();
    void initApp();
    void initMainBar();

    enum class Actions : uint8_t{
        AddWorker,
        DeleteWorker
    };
    constexpr size_t at(const Actions value) const{
        return static_cast<size_t>(value);
    }

    std::vector<QAction*> mainBarActions;
};
#endif // MAINWINDOW_H
