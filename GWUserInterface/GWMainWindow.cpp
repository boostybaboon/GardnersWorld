#include "GWMainWindow.h"
#include "GW3dViewWidget.h"
#include "../GWController/GWController.h"
#include "../GWView/GWView.h"

#include <QtCore/QTimer>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <iostream>

GWMainWindow::GWMainWindow(QWidget* wparent, int argc, char* argv[]) :
    QMainWindow(wparent)
    , _timer(0)
    , _gw3dViewWidget(0)
    , _gwController(0)
{
    setupUi(this);

    //you can control enable/disable state of actions like so...
    //connect(child, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    //or
    //connect(other, SIGNAL(someSig(type)), this, SLOT(updateMenus(type)));
}

GWMainWindow::~GWMainWindow()
{
    closeController();
}

void GWMainWindow::on_actionNew_activated()
{
    on_actionClose_activated();
    
    //plumb the view and the browser in to each other - this needs doing
    //a bit more oo eventually, the code below is a bit handfisted
    
    //make a new controller so we've got something to give top level commands to
    _gwController = QSharedPointer<GWController>(new GWController);
    
    //make a new widget to view the 3d in and set it as the central widget
    _gw3dViewWidget = new GW3dViewWidget;
    setCentralWidget(_gw3dViewWidget);
    
    //open up a new view on the scene and attach it to the view widget
    QSharedPointer<GW3dView> gw3dView = _gwController->openNew3dView();
    _gw3dViewWidget->setGW3dView(gw3dView);
    
    //make a timer, connect it up and start it
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer->start(20);

    //by the time we get here, we should have a single view on an empty model
}

void GWMainWindow::on_actionOpen_activated()
{
    on_actionClose_activated();
    
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Some Shit!"),
                                                    "/home/ian/www.web3d.org/x3d/content/examples",
                                                    tr("Gardner's world files (*.gwx);;all files (*.*)"));
                                                    
    if(fileName.isEmpty())
    {
        return;
    }
                                                    
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::on_actionSave_activated()
{
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::on_actionSave_As_activated()
{
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::on_actionClose_activated()
{
    //test whether document needs saving first...

    closeController();
}

void GWMainWindow::on_actionQuit_activated()
{
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::on_actionUndo_activated()
{
    _gwController->undo();
}

void GWMainWindow::on_actionRedo_activated()
{
    _gwController->redo();
}

void GWMainWindow::on_actionModelAdd_activated()
{
    std::cout << "open a file..." << std::endl;
    
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Add a model!"),
                                                    "/home/ian/www.web3d.org/x3d/content/examples",
                                                    tr("osg files (*.osg *.3ds);;all files (*.*)"));

    if(fileName.isEmpty())
    {
        return;
    }

    GWAddModelCommand c(*_gwController);
    c.setModelName(fileName);
    _gwController->doCommand(c);
    return;
}

void GWMainWindow::on_actionModelEdit_activated()
{
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::on_actionModelDelete_activated()
{
    QMessageBox::information(this, tr("Implement..."), tr("...me!"));
}

void GWMainWindow::update()
{
    if(_gwController)
    {
        _gwController->tick();
    }
}

void GWMainWindow::updateMenus()
{
    //you can control enable/disable state of menu items thusly...
    //    bool hasMdiChild = (activeMdiChild() != 0);
    //    saveAct->setEnabled(hasMdiChild);
    //    saveAsAct->setEnabled(hasMdiChild);
    //    etc...

    
}

void GWMainWindow::closeController()
{
    if(_timer)
    {
        _timer->stop();
        disconnect(_timer, SIGNAL(timeout()), this, SLOT(update()));
    }
    
    delete _timer;
    _timer = 0;
    
    delete _gw3dViewWidget;
    _gw3dViewWidget = 0;
    
    _gwController.clear();
}
