#include "editrecord.h"
#include "ui_editrecord.h"

EditRecord::EditRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditRecord)
{
    ui->setupUi(this);
}

EditRecord::~EditRecord()
{
    delete ui;
}
