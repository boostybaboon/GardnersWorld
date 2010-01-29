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

#include "GWCommand.h"
#include "../GWDocument/GWNode/GWModel.h"
#include "../GWDocument/GWDocument.h"
#include "../GWView/GWView.h"
#include "../GWView/GWCommand3dView.h"

#include <QtCore/QString>
#include "GWController.h"

GWCommand::GWCommand()
    : _isRedoing(true)
{
    
}

GWCommand::GWCommand(QDomElement commandElement, bool isRedo)
    : _commandElement(commandElement) 
    , _isRedoing(isRedo)
{
    
}

GWCommand::~GWCommand()
{

}

bool GWCommand::redo(GWDocument& d)
{
    //this probably needs to be a try/catch, as DoImpl could fail in all sorts of
    //interesting ways deep in interesting stacks, where throwing would probably
    //be the easiest way to break out, rather than passing falses up the ladder
    bool doSuccess = redoImpl(d);
    if(doSuccess)
    {
        d.commandDone(*this);
    }
    return doSuccess;
}

bool GWCommand::undo(GWDocument& d)
{
    _isRedoing = false;
    
    bool undoSuccess = undoImpl(d);
    if(undoSuccess)
    {
        d.commandDone(*this);
    }
    return undoSuccess;
}

QDomElement GWCommand::getCommandElement(GWDocument& d)
{
    return _commandElement;
}

bool GWCommand::isRedoing() const
{
    return _isRedoing;
}

const QString GWAddModelCommand::commandId = "GWAddModelCommand";

/**
 * Constructor for first time "do" of a command
 */
GWAddModelCommand::GWAddModelCommand(GWController& gwc)
{
    //set up the _commandElement member
    GWDocument& gwDoc = gwc.getGWDocument();
    _commandElement = gwDoc.newElement(commandId);
    _commandElement.setAttribute("url","");
    _commandElement.setAttribute("globalId","-1");
}

/**
* Constructor for making command from command stack element
*/
GWAddModelCommand::GWAddModelCommand(QDomElement commandElement, bool isRedo)
    : GWCommand(commandElement,isRedo)
{
}

GWAddModelCommand::~GWAddModelCommand()
{
}

void GWAddModelCommand::setModelName(const QString& name)
{
    _commandElement.setAttribute("url",name);
}

QString GWAddModelCommand::getModelName() const
{
    return _commandElement.attribute("url");
}

bool GWAddModelCommand::redoImpl(GWDocument& d)
{
    bool ok;
    int globId = _commandElement.attribute("globalId","-1").toInt(&ok);

    if(ok)
    {
        _gwModel = QSharedPointer<GWModel>(new GWModel(d, globId));

        //no need to make a note of the pre command position in this case

        if(globId < 0)
        {
            _commandElement.setAttribute("globalId", _gwModel->getGlobalId());
        }

        _gwModel->setUrl(_commandElement.attribute("url"));

        //gwModel is now set up, it can be added to the scene
        _gwModel->addToScene(d,GWSceneGraphPosition(d.getSceneNodeId()));

        return true;
    }

    return false;
}

bool GWAddModelCommand::undoImpl(GWDocument& d)
{
    //find the global id of the model
    bool ok;
    int globId = _commandElement.attribute("globalId","-1").toInt(&ok);
    
    if(ok && globId > -1)
    {
        //find the scene node with the corresponding global id
        _gwModel = QSharedPointer<GWModel>(new GWModel(d, globId));

        //make a note of its position
        _preCommandPos = _gwModel->getSceneGraphPosition();
        
        //remove it!
        _gwModel->removeFromScene(d);

        return true;
    }

    return false;
}

const QString& GWAddModelCommand::getStringId() const
{
    return commandId;
}

QVector<AffectedNodeInfo> GWAddModelCommand::getAffectedNodes() const
{
    AffectedNodeInfo ani;
    ani._affectedNode = _gwModel;
    ani._preCommandPos = _preCommandPos;

    QVector<AffectedNodeInfo> affectedNodes;
    affectedNodes.append(ani);
    return affectedNodes;
}

//const GWModel& GWAddModelCommand::getGWModel() const
//{
//    return *_gwModel;
//}

QSharedPointer<GWCommand3dView> GWAddModelCommand::getCommand3dView() const
{
    return QSharedPointer<GWCommand3dView>(new GWAddModelCommand3dView(*this));
}
