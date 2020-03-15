#ifndef NEWWORKER_H
#define NEWWORKER_H

#include <QDialog>
class QStringList;

namespace Ui {
class NewWorker;
}

class NewWorker : public QDialog
{
    Q_OBJECT

public:
    explicit NewWorker(QWidget *parent = nullptr);
    ~NewWorker();
    QString getComment();
    QString getFirsName();
    QString getLastName();
    QString getPhoneNumber();
    QDateTime getNextDateTime();
    uint16_t getVacancyIndex();
    void setVacancies(const QStringList & vac);
    void setMaxCommentSize(const uint16_t &size);

private:
    Ui::NewWorker *ui;
    uint16_t maxCommentSize = 0;
private slots:
    void checkInput();
    void checkCommentInput();
};

#endif // NEWWORKER_H
