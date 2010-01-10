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

#ifndef _GWDOCUMENT_H
#define _GWDOCUMENT_H

#include "../GWController/GWCommand.h"

#include <QtXml/QDomElement>

#include <string>
#include <QtCore/QObject>

class GWObserver;

class GWSubject : public QObject
{
    Q_OBJECT

public:
    virtual ~GWSubject() {}
    
    void registerObserver(GWObserver& subscriber) const;
    void unregisterObserver(GWObserver& subscriber) const;

signals:
    virtual void notify(const GWCommand& m);
};

#include "GWDom.h"

class GWDocument : public GWSubject
{
public:
    GWDocument();
    virtual ~GWDocument();

    QDomElement getDocumentElement();
    QDomElement getSceneElement();
    QDomElement getCommandsElement();
    QDomElement newElement(const QString& elementName);
    void commandDone(GWCommand& gwc);
    int generateGlobalId();
    QSharedPointer<GWCommand> getCurrentCommand();
    QSharedPointer<GWCommand> getNextCommand();
    
private:
    QSharedPointer<GWCommand> getCommand(QDomElement commandElement, bool isRedo);
    void updateCommandStack(GWCommand& gwc);

    QDomElement _currentCommand;
    QDomElement _sceneElement;
    QDomElement _commandsElement;
    
    int _globIdCounter;
    GWDom _gwDom;
};

#endif // _GWDOCUMENT_H
