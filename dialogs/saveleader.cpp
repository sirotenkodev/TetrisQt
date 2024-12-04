#include "saveleader.h"
#include "ui_saveleader.h"

//-----------------------------------------------------------------------------
SaveLeader::SaveLeader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveLeader)
{
    ui->setupUi(this);
}

//-----------------------------------------------------------------------------
SaveLeader::~SaveLeader()
{
    delete ui;
}

//-----------------------------------------------------------------------------
std::string SaveLeader::getName()
{
    return ui->nameEdit->text().toStdString();
}
