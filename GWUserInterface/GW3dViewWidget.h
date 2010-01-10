#ifndef _GWVIEWWIDGET_H_
#define _GWVIEWWIDGET_H_

#include <QtGui/QWidget>
#include <QtCore/QSharedPointer>

class GW3dView;

class GW3dViewWidget : public QWidget
{
public:
    GW3dViewWidget(QWidget* wparent = 0, const char* name = 0, Qt::WindowFlags f = 0 );
    virtual ~GW3dViewWidget();

    void setGW3dView(QSharedPointer<GW3dView> gw3dv);

private:
    virtual void closeEvent(QCloseEvent * evnt);
    virtual void destroyEvent(bool destroyWindow = true, bool destroySubWindows = true);
    virtual void resizeEvent(QResizeEvent * evnt);
    virtual void keyPressEvent(QKeyEvent* evnt);
    virtual void keyReleaseEvent(QKeyEvent* evnt);
    virtual void mousePressEvent(QMouseEvent* evnt);
    virtual void mouseReleaseEvent(QMouseEvent* evnt);
    virtual void mouseDoubleClickEvent(QMouseEvent* evnt);
    virtual void mouseMoveEvent(QMouseEvent* evnt);
    
    QSharedPointer<GW3dView> _gw3dView;
};

#endif
