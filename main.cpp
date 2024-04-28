#include "Annotation_Tool_Main.h"
#include "Annotation_FIle.h"
#include"Annotation_Tool_login.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Annotation_Tool_Main main;
    Annotation_Tool_login login;

    //QObject::connect(&login, &Annotation_Tool_login::login_success, [&]() {
    //    login.close();
    //    main.show();
    //    });
    
    login.show();
    //main.show();
    return a.exec();
}
