#ifndef EDITRECORD_H
#define EDITRECORD_H

#include <QDialog>

namespace Ui {
class EditRecord;
}

class EditRecord : public QDialog
{
    Q_OBJECT

public:
    explicit EditRecord(QWidget *parent = nullptr);
    ~EditRecord();

private:
    Ui::EditRecord *ui;
};

#endif // EDITRECORD_H
