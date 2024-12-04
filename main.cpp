#include <QApplication>

#include "gui/menuform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuForm form;
    form.show();
    return a.exec();
}
