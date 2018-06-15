#include "notepad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Notepad pad;
    pad.show();

    return app.exec();
}
