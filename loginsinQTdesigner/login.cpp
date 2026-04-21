#include "login.h"
#include <QApplication>
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    LoginWindow ventana;
    ventana.show();
    return app.exec();
}
