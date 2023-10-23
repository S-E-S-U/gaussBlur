#include "qtsmth.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{ 
    QApplication a(argc, argv);
    QtSmth w;
   
    w.show();
    
    return a.exec();
}
