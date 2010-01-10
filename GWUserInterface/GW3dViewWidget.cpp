#include "GW3dViewWidget.h"

#include <../GWView/GWView.h>

#include <QtGui/QResizeEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>

GW3dViewWidget::GW3dViewWidget(QWidget* wparent, const char* name, Qt::WindowFlags f)
    : QWidget(wparent, f)
    , _gw3dView(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
}

GW3dViewWidget::~GW3dViewWidget()
{

}

void GW3dViewWidget::setGW3dView(QSharedPointer<GW3dView> gw3dv)
{
    _gw3dView = gw3dv;
    
    _gw3dView->initialise(winId(),x(),y(),width(),height());
}

void GW3dViewWidget::closeEvent(QCloseEvent * evnt)
{
    if(_gw3dView)
    {
        _gw3dView->closeWindow();
    }
}

void GW3dViewWidget::destroyEvent(bool destroyWindow, bool destroySubWindows)
{
    if(_gw3dView)
    {
        _gw3dView->closeWindow();
    }
}

void GW3dViewWidget::resizeEvent(QResizeEvent * evnt)
{
    if(_gw3dView)
    {
        const QSize& theSize = evnt->size();
        _gw3dView->windowResize(theSize.width(),theSize.height());
    }
}

void GW3dViewWidget::keyPressEvent(QKeyEvent* evnt)
{
    if(_gw3dView)
    {
        int c = *evnt->text().toAscii().data();
        _gw3dView->keyPress(c);
    }
}

void GW3dViewWidget::keyReleaseEvent(QKeyEvent* evnt)
{
    if(_gw3dView)
    {
        int c = *evnt->text().toAscii().data();
        _gw3dView->keyRelease(c);
    }
}

void GW3dViewWidget::mousePressEvent(QMouseEvent* evnt)
{
    if(_gw3dView)
    {
        int button = 0;
        switch(evnt->button())
        {
            case(Qt::LeftButton): button = 1; break;
            case(Qt::MidButton): button = 2; break;
            case(Qt::RightButton): button = 3; break;
            case(Qt::NoButton): button = 0; break;
            default: button = 0; break;
        }	
        _gw3dView->mouseButtonPress(evnt->x(), evnt->y(), button);
    }
}

void GW3dViewWidget::mouseReleaseEvent(QMouseEvent* evnt)
{
    if(_gw3dView)
    {
        int button = 0;
        switch(evnt->button())
        {
            case(Qt::LeftButton): button = 1; break;
            case(Qt::MidButton): button = 2; break;
            case(Qt::RightButton): button = 3; break;
            case(Qt::NoButton): button = 0; break;
            default: button = 0; break;
        }
        _gw3dView->mouseButtonRelease(evnt->x(), evnt->y(), button);
    }
}

void GW3dViewWidget::mouseDoubleClickEvent(QMouseEvent* evnt)
{
    if(_gw3dView)
    {
        int button = 0;
        switch(evnt->button())
        {
            case(Qt::LeftButton): button = 1; break;
            case(Qt::MidButton): button = 2; break;
            case(Qt::RightButton): button = 3; break;
            case(Qt::NoButton): button = 0; break;
            default: button = 0; break;
        }
        _gw3dView->mouseDoubleButtonPress(evnt->x(), evnt->y(), button);
    }
}

void GW3dViewWidget::mouseMoveEvent(QMouseEvent* evnt)
{
    if(_gw3dView)
    {
        _gw3dView->mouseMotion(evnt->x(), evnt->y());
    }
}
