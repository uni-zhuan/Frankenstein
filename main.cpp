#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Frankenstein");
    w.setWindowIcon(QIcon(":/icons/Frankenstein.png"));
    w.show();
    return a.exec();
}
