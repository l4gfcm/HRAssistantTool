#ifndef MANAGEVACANCIES_H
#define MANAGEVACANCIES_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class ManageVacancies;
}

class ManageVacancies : public QDialog
{
    Q_OBJECT

public:
    explicit ManageVacancies(QWidget *parent = nullptr);
    ~ManageVacancies();
    enum class CommandType
    {
        None, Add, Delete
    };
    CommandType command = CommandType::None;
    void setVacancies(const QStringList &vacancies);
    std::list<uint16_t> getSelectedItems();
private:
    Ui::ManageVacancies *ui;
    std::vector<QCheckBox *> vacList;
private slots:
    void deleteVacancy();
    void addVacancy();
};

#endif // MANAGEVACANCIES_H
