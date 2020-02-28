#ifndef EDITRECORD_H
#define EDITRECORD_H

#include <QDialog>
#include <vector>
class pair;
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
    void setSteps (const std::vector<std::pair<uint16_t, QString>> &steps);
    QDateTime getNextDate();
    QString getComment();

private:
    Ui::EditRecord *ui;
    std::vector<QRadioButton *> buttons;
};

#endif // EDITRECORD_H
