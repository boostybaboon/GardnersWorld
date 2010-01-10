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

GWModel::GWModel(GWDocument& gwd, int globalId /* = -1 */)
{
    QDomElement modelElement;

    if(globalId > -1)
    {
        //fetch existing item, if such exists
        QString sGlobId = QString::number(globalId);
        modelElement = QDomXmlModel::selectNode(gwd.getSceneElement(), "//GWModel[@globalId=\'" + sGlobId + "\']").toElement();
        if(modelElement.isNull())
        {
            globalId = -1;
        }
    }

    if(globalId < 0)
    {
        //make a new unparented dom node <GWModel url="path to file" globalId="1234" />
        modelElement = gwd.newElement(QString("GWModel"));
        modelElement.setAttribute(QString("url"), QString(""));
        modelElement.setAttribute(QString("globalId"), gwd.generateGlobalId());
    }
    
    _gwModelElement = modelElement;
}

GWModel::GWModel(QDomElement modelElement)
{
    _gwModelElement = modelElement;
}

GWModel::~GWModel()
{

}

void GWModel::setUrl(const QString& url)
{
    _gwModelElement.setAttribute(QString("url"), url);
}

const QString GWModel::getUrl() const
{
    return _gwModelElement.attribute("url");
}

int GWModel::getGlobalId() const
{
    const QString idstr = _gwModelElement.attribute("globalId");
    return idstr.toInt();
}

void GWModel::addToScene(GWDocument& gwd)
{
    gwd.getSceneElement().appendChild(_gwModelElement);
}

void GWModel::removeFromScene(GWDocument& gwd)
{
    _gwModelElement.parentNode().removeChild(_gwModelElement);
}
