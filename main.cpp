#include "my_window.h"
#include "projeto.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow w;

    w.setWindowTitle("Pesquisa");

    w.show();
    return a.exec();
}
