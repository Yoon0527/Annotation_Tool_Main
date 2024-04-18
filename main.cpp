#include "Annotation_Tool_Main.h"
#include "Annotation_FIle.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Annotation_Tool_Main w;
    w.show();
    return a.exec();
}
