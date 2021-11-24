#include "subwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    subWidget w;
    w.show();
    return a.exec();
}
