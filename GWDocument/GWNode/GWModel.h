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

#ifndef GWMODEL_H
#define GWMODEL_H

#include <QtXml/QDomElement>
#include <QtCore/QSharedPointer>
#include <QtCore/QPair>

class GWDocument;
class GWNode3dView;
class GW3dView;

class GWSceneGraphPosition
{
public:
    GWSceneGraphPosition() : _pair(-1,-1) {}
    GWSceneGraphPosition(int theParent, int theNextSibling = -1) : _pair(theParent,theNextSibling) {}

    GWSceneGraphPosition& operator=(const GWSceneGraphPosition& pos)
    {
        if(&pos==this) return *this;
        _pair = pos._pair;
        return *this;
    }
    int& parent() { return _pair.first; }
    int& nextSibling() { return _pair.second; }

private:
    QPair<int,int> _pair;
};

std::ostream& operator << (std::ostream& os, GWSceneGraphPosition& pos);

class GWNode
{
public:
    GWNode();
    virtual ~GWNode();

    virtual QSharedPointer<GWNode3dView> getNode3dView(GW3dView& gw3dv) const = 0;
    virtual void addToScene(GWDocument& gwd, GWSceneGraphPosition pos);
    virtual void addToScene(GWDocument& gwd, GWNode& parent, GWNode& nextSibling);
    virtual void removeFromScene(GWDocument& gwd);

    GWSceneGraphPosition getSceneGraphPosition() const;
    static int getGlobalIdFromElement(QDomElement elem);
    int getGlobalId() const;

protected:
    virtual void addToScene(GWDocument& gwd, QDomElement parent, QDomElement nextSibling);

    QDomElement _domElement;
};

class GWModel : public GWNode
{
public:
    GWModel(GWDocument& gwd, int globalId = -1);
    GWModel(QDomElement modelElement);
    virtual ~GWModel();

    QSharedPointer<GWNode3dView> getNode3dView(GW3dView& gw3dv) const;

    void setUrl(const QString& url);
    const QString getUrl() const;

private:
};

#endif // GWMODEL_H
