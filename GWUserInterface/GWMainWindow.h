#ifndef _GWMAINWINDOW_H_
#define _GWMAINWINDOW_H_

#include "ui_GWMainWindowUi.h"

#include <QtCore/QSharedPointer>

class GW3dViewWidget;
class GWController;
class QTimer;

class GWMainWindow : public QMainWindow,
                     private Ui::GWMainWindowUi
{
Q_OBJECT

public:
    GWMainWindow(QWidget* wparent, int argc, char* argv[]);
    virtual ~GWMainWindow();

private slots:

    //file
    void on_actionNew_activated();
    void on_actionOpen_activated();
    void on_actionSave_activated();
    void on_actionSave_As_activated();
    void on_actionClose_activated();
    void on_actionQuit_activated();
    
    //edit
    void on_actionUndo_activated();
    void on_actionRedo_activated();
    
    //interaction
    void on_actionTrackball_activated();
    void on_actionSelect_activated();
    void on_actionManipulate_activated();

    //commands
    void on_actionModelAdd_activated();
    void on_actionModelEdit_activated();
    void on_actionModelDelete_activated();

    //window

    //help

    //update function for connection to timer
    void update();

    //update menu items
    void updateMenus();
    
private:
    void closeController();
    GW3dViewWidget* getActive3dViewWidget();
    
    //can you hold QTimer and QWidget pointers in QSharedPointer? 
    QTimer* _timer;
    GW3dViewWidget* _gw3dViewWidget;
    QSharedPointer<GWController> _gwController;
};

#endif
