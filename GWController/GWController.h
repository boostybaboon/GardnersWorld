#ifndef _GWCONTOLLER_H_
#define _GWCONTOLLER_H_

#include <QtCore/QSharedPointer>

class GWDocument;
class GW3dView;
class GWCommand;
class GWAddModelCommand;

class GWController
{
public:
    GWController();
    virtual ~GWController();

    QSharedPointer<GW3dView> openNew3dView();

    void doCommand(GWCommand& command);

    void undo();
    void redo();
    
    void tick();

    GWDocument& getGWDocument();

private:
    QSharedPointer<GWDocument> _gwDocument;
    QSharedPointer<GW3dView> _gw3dView;
};

#endif
