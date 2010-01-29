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

#include "GWModel.h"
#include "../GWDocument.h"
#include "../GWDom.h"
#include "../qdomxmlmodel.h"
#include "../../GWView/GWView.h"
#include "../../GWView/GWModel3dView.h"

std::ostream& operator << (std::ostream& os, GWSceneGraphPosition& pos)
{
    os << "Parent: " << pos.parent();
    os << " NextSibling: " << pos.nextSibling();

    return os;
}

GWNode::GWNode()
{

}

GWNode::~GWNode()
{

}

/**
 * Assume that every GWNode wraps a dom element which has a parent
 * element which has a global id attribute, and if it has a next
 * sibling then this too will have a global id attribute
 */
GWSceneGraphPosition GWNode::getSceneGraphPosition() const
{
    QDomElement parentElement = _domElement.parentNode().toElement();
    QDomElement nextSiblingElement = _domElement.nextSiblingElement();

    int parentId = getGlobalIdFromElement(parentElement);
    int nextSiblingId = getGlobalIdFromElement(nextSiblingElement);

    return GWSceneGraphPosition(parentId,nextSiblingId);
}

int GWNode::getGlobalId() const
{
    return getGlobalIdFromElement(_domElement);
}

/* static */
int GWNode::getGlobalIdFromElement(QDomElement elem)
{
    int id = -1;

    if(!elem.isNull() && elem.hasAttribute("globalId"))
    {
        const QString idstr = elem.attribute("globalId");
        id = idstr.toInt();
    }

    return id;
}

void GWNode::addToScene(GWDocument& gwd, GWSceneGraphPosition pos)
{
    gwd.getSceneElement().appendChild(_domElement);
}

void GWNode::addToScene(GWDocument& gwd, GWNode& parent,  GWNode& nextSibling)
{
    addToScene(gwd, parent._domElement, nextSibling._domElement);
}

/**
 * Beware, behaviour of this function is contrary to what you'd expect from
 * QDomNode::insertBefore which this function uses. If nextSibling is null, the
 * member node is inserted as the last child of parent.
 */
void GWNode::addToScene(GWDocument& gwd, QDomElement parent, QDomElement nextSibling)
{
    if(nextSibling.isNull())
    {
        parent.insertAfter(_domElement,QDomElement());
    }
    else
    {
        parent.insertBefore(_domElement,nextSibling);
    }
}

void GWNode::removeFromScene(GWDocument& gwd)
{
    _domElement.parentNode().removeChild(_domElement);
}

GWModel::GWModel(GWDocument& gwd, int globalId /* = -1 */)
{
    QDomElement modelElement;

    if(globalId > -1)
    {
        //fetch existing item, if such exists
        QString sGlobId = QString::number(globalId);
        modelElement = QDomXmlModel::selectNode(gwd.getSceneElement(), "//GWModel[@globalId=\'" + sGlobId + "\']").toElement();
    }

    if(modelElement.isNull())
    {
        //make a new unparented dom node <GWModel url="path to file" globalId="1234" />
        modelElement = gwd.newElement(QString("GWModel"));
        modelElement.setAttribute(QString("url"), QString(""));
        int globId = (globalId > -1) ? globalId : gwd.generateGlobalId();
        modelElement.setAttribute(QString("globalId"), globId);
    }
    
    _domElement = modelElement;
}

GWModel::GWModel(QDomElement modelElement)
{
    _domElement = modelElement;
}

GWModel::~GWModel()
{

}

QSharedPointer<GWNode3dView> GWModel::getNode3dView(GW3dView& gw3dv) const
{
    return QSharedPointer<GWNode3dView>(new GWModel3dView(gw3dv, *this));
}

void GWModel::setUrl(const QString& url)
{
    _domElement.setAttribute(QString("url"), url);
}

const QString GWModel::getUrl() const
{
    return _domElement.attribute("url");
}
