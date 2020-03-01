#ifndef EDITRECORD_H
#define EDITRECORD_H

#include <QDialog>
#include <vector>

class QRadioButton;
class QButtonGroup;
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
    void setHistory(const std::vector<std::tuple<QString, QString, QDateTime>> &);
private:
    Ui::EditRecord *ui;
    //std::vector<QRadioButton *> buttons;
    QButtonGroup * buttonsGroup = nullptr;
private slots:
    void checkInput();
};

#endif // EDITRECORD_H
