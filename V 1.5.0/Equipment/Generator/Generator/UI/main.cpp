#include "GeneratorUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GeneratorUI w;
    w.show();

    return a.exec();
}
