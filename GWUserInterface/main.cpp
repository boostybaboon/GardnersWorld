#include "GWMainWindow.h"
#include <QtGui/QApplication>

/**
 * Simple main function to start the main window
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    GWMainWindow mainWin(0,argc,argv);
    mainWin.show();
    return app.exec();
}

