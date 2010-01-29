/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "GWDocument.h"
#include "qdomxmlmodel.h"
#include "GWDom.h"
#include "GWNode/GWModel.h"
#include "../GWView/GWView.h"

void GWSubject::registerObserver(GWObserver& subscriber) const
{
    connect(this, SIGNAL(notify(const GWCommand&)), &subscriber, SLOT(update(const GWCommand&)));
}

void GWSubject::unregisterObserver(GWObserver& subscriber) const
{
    disconnect(this, SIGNAL(notify(const GWCommand&)), &subscriber, SLOT(update(const GWCommand&)));
}


GWDocument::GWDocument()
    : _globIdCounter(0)
    , _sceneNodeId(-1)
{
    _gwDom.newDomDocument("GardnersWorld");
    QDomElement domElem = _gwDom.getDocumentElement();
    
    _sceneElement = _gwDom.newElement("Scene");
    _sceneNodeId = generateGlobalId();
    _sceneElement.setAttribute("globalId",_sceneNodeId);
    domElem.appendChild(_sceneElement);
    _commandsElement = _gwDom.newElement("Commands");
    domElem.appendChild(_sceneElement);
}

GWDocument::~GWDocument()
{
}

QDomElement GWDocument::newElement(const QString& elementName)
{
    return _gwDom.newElement(elementName);
}

QDomElement GWDocument::getDocumentElement()
{
    return _gwDom.getDocumentElement();
}

QDomElement GWDocument::getSceneElement()
{
    return _sceneElement;
}

int GWDocument::getSceneNodeId() const
{
    return _sceneNodeId;
}

QDomElement GWDocument::getCommandsElement()
{
    return _commandsElement;
}

int GWDocument::generateGlobalId()
{
    return _globIdCounter++;
}

void GWDocument::updateCommandStack(GWCommand& gwc)
{
    QDomElement commandElement = gwc.getCommandElement(*this);

    if(commandElement.parentNode().isNull())
    {
        //if no parent, assume it needs adding to command stack in the DOM
        //(otherwise we should be doing an undo or redo, in which case this
        //command node will already be part of the commands part of the DOM)
        //delete all nodes after this one if a new command is going into
        //the undo stack
        QDomElement nextCommandElement = _currentCommand.nextSiblingElement();
        while(!nextCommandElement.isNull())
        {
            QDomElement nextNextCommandElement = nextCommandElement.nextSiblingElement();
            _commandsElement.removeChild(nextCommandElement);
            nextCommandElement = nextNextCommandElement;
        }
        _commandsElement.appendChild(commandElement);
    }

    if(gwc.isRedoing())
    {
        _currentCommand = commandElement;
    }
    else
    {
        _currentCommand = commandElement.previousSiblingElement();
    }
}

void GWDocument::commandDone(GWCommand& gwc)
{
    updateCommandStack(gwc);
    notify(gwc);
}

QSharedPointer<GWCommand> GWDocument::getCurrentCommand()
{
    return getCommand(_currentCommand, false);
}

QSharedPointer<GWCommand> GWDocument::getNextCommand()
{
    QDomElement commandElement;
    
    if(!_currentCommand.isNull())
    {
        commandElement = _currentCommand.nextSiblingElement();
    }
    else
    {
        commandElement = _commandsElement.firstChildElement();
    }

return getCommand(commandElement, true);
}

/**
 * This needs to be a better factory function - need some way of registering commands with the document, so that we
 * can do a string based look up of an object to create based on the element's name
 */
QSharedPointer<GWCommand> GWDocument::getCommand(QDomElement commandElement, bool isRedo)
{
    if(!commandElement.isNull())
    {
        QString commandType = commandElement.tagName();

        if(QString::compare(commandType, GWAddModelCommand::commandId, Qt::CaseInsensitive) == 0)
        {
            return QSharedPointer<GWCommand>(new GWAddModelCommand(commandElement, isRedo));
        }
    }
    
    return QSharedPointer<GWCommand>(0);
}
