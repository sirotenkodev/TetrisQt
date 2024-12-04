#ifndef SAVELEADER_H
#define SAVELEADER_H

#include <string>

#include <QDialog>

namespace Ui {
class SaveLeader;
}

class SaveLeader : public QDialog
{
    Q_OBJECT

public:
    explicit SaveLeader(QWidget *parent = nullptr);
    ~SaveLeader();

    std::string getName();

private:
    Ui::SaveLeader *ui;
};

#endif // SAVELEADER_H
