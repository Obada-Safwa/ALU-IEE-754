#include "mainwindow.h"

#include <QApplication>

float giveAlwaysplusone(float p){
    return p +1;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Calculadora Decimal - IEEE 754");
    w.show();

    return a.exec();
}
