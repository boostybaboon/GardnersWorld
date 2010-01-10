#include "GWController.h"

#include "GWCommand.h"
#include "../GWView/GWView.h"
#include "../GWDocument/GWDocument.h"

GWController::GWController()
    : _gwDocument(new GWDocument)
    , _gw3dView(0)
{

}

GWController::~GWController()
{

}


QSharedPointer<GW3dView> GWController::openNew3dView()
{
    //not sure whether anything more sophisticated needs doing here...
    //we probably need to keep track of it in an array of views

    //maybe this needs to be a factory function eventually if we're holding
    //many different kinds of views

    //also probably need to register the view and document with each other somehow, as the
    //document is created before any views are opened up on it
    
    QSharedPointer<GW3dView> gw3dView(new GW3dView(*_gwDocument));

    //this should be an array eventually
    _gw3dView = gw3dView;

    return _gw3dView;
}

void GWController::doCommand(GWCommand& command)
{
    command.redo(*_gwDocument);
}

void GWController::undo()
{
    //1. get a command based on the current command stack item
    QSharedPointer<GWCommand> gwc(_gwDocument->getCurrentCommand());

    //2. execute the undo function on the command, which acts on the document
    if(gwc)
    {
        gwc->undo(*_gwDocument);
    }
}

void GWController::redo()
{
    //1. get a command based on the current command stack item
    QSharedPointer<GWCommand> gwc(_gwDocument->getNextCommand());
    
    //2. execute the redo function on the command, which acts on the document
    if(gwc)
    {
        gwc->redo(*_gwDocument);
    }
}

void GWController::tick()
{
    _gw3dView->tick();
}

GWDocument& GWController::getGWDocument()
{
    return *_gwDocument;
}
