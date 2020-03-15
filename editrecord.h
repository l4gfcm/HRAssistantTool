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

    void setName(const std::pair<QString, QString> &);
    void setSteps (const QStringList &steps);
    QDateTime getNextDate();
    QString getComment();
    uint16_t getStateId();
    void setHistory(const std::vector<std::tuple<QString, QString, QDateTime>> &);
    void setMaxCommentSize(const uint16_t &size);
private:
    Ui::EditRecord *ui;
    QButtonGroup * buttonsGroup = nullptr;
    uint16_t maxCommentSize = 0;
private slots:
    void checkInput();
    void checkCommentInput();
};

#endif // EDITRECORD_H
