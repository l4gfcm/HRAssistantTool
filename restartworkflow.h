#ifndef RESTARTWORKFLOW_H
#define RESTARTWORKFLOW_H

#include <QDialog>

namespace Ui {
class RestartWorkflow;
}

class RestartWorkflow : public QDialog
{
    Q_OBJECT

public:
    explicit RestartWorkflow(QWidget *parent = nullptr);
    ~RestartWorkflow();
    void setVacancies(const QStringList & vacancies, const uint16_t &currentVacancy);
    uint16_t getVacancy();
    QDateTime getNextDate();
    void setWorkerName(const std::pair<QString, QString> &name);

private:
    Ui::RestartWorkflow *ui;
};

#endif // RESTARTWORKFLOW_H
