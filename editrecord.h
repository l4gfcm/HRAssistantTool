#ifndef EDITRECORD_H
#define EDITRECORD_H

#include <QDialog>
#include <vector>

class QRadioButton;

namespace Ui {
class EditRecord;
}

class EditRecord : public QDialog
{
    Q_OBJECT

public:
    explicit EditRecord(QWidget *parent = nullptr);
    ~EditRecord();

    void setFLName(const QString &flname);
    void setSteps (const std::vector<QString> &steps);
    QDateTime getNextDate();
    QString getComment();
    uint16_t getStateId();
private:
    Ui::EditRecord *ui;
    std::vector<QRadioButton *> buttons;
private slots:
    void checkInput();
};

#endif // EDITRECORD_H
