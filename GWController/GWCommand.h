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

#ifndef GWCOMMAND_H
#define GWCOMMAND_H

#include <QtXml/QDomElement>
#include <QtCore/QSharedPointer>

class GWController;
class GWDocument;
class GWModel;
class GW3dView;
class GWCommand3dView;

class GWCommand
{
public:
    GWCommand();
    GWCommand(QDomElement commandElement, bool isRedo);
    virtual ~GWCommand();

    bool redo(GWDocument& d);
    bool undo(GWDocument& d);

    virtual const QString& getStringId() const = 0;

    virtual QSharedPointer<GWCommand3dView> getCommand3dView() const = 0;
    
    QDomElement getCommandElement(GWDocument& d);

    bool isRedoing() const;

protected:
    QDomElement _commandElement;
    
private:
    virtual bool redoImpl(GWDocument& d) = 0;
    virtual bool undoImpl(GWDocument& d) = 0;

    bool _isRedoing;
};

class GWAddModelCommand : public GWCommand
{
public:
    GWAddModelCommand(GWController& gwm);
    GWAddModelCommand(QDomElement commandElement, bool isRedoing);
    virtual ~GWAddModelCommand();

    void setModelName(const QString& name);
    QString getModelName() const;

    const QString& getStringId() const;

    const GWModel& getGWModel() const;
    
    QSharedPointer<GWCommand3dView> getCommand3dView() const;

    static const QString commandId;

private:
    virtual bool redoImpl(GWDocument& d);
    virtual bool undoImpl(GWDocument& d);

    QSharedPointer<GWModel> _gwModel;
};

#endif // GWCOMMAND_H
