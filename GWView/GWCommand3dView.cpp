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

#include "GWCommand3dView.h"

#include "../GWController/GWCommand.h"
#include "GWModel3dView.h"
#include "../GWDocument/GWNode/GWModel.h"

GWCommand3dView::GWCommand3dView(const GWCommand& gwc)
    : _gwCommand(gwc)
{

}

GWCommand3dView::~GWCommand3dView()
{

}

/* Default implementation for updating a 3d view is to get the list of
 * nodes affected by the associated GWCommand, remove them from the
 * 3d view and re-add them. For more sophisticated updates or for where
 * removal/re-addition is overkill, reimplement this virtual function
 * in the derived GWCommand3dView class
 */
void GWCommand3dView::update3dView(GW3dView& gw3dv) const
{
    QVector<AffectedNodeInfo> affectedNodes = _gwCommand.getAffectedNodes();

    QVectorIterator<AffectedNodeInfo> i(affectedNodes);
    while(i.hasNext())
    {
        AffectedNodeInfo ani = i.next();
        QSharedPointer<GWNode> gwNode = ani._affectedNode;
        GWSceneGraphPosition preCommandPos = ani._preCommandPos;
        QSharedPointer<GWNode3dView> gwn3dv = gwNode->getNode3dView(gw3dv);

        gwn3dv->removeFrom3dView(preCommandPos);

        gwn3dv->addTo3dView();
    }
}

GWAddModelCommand3dView::GWAddModelCommand3dView(const GWAddModelCommand& gwc)
    : GWCommand3dView(gwc)
    , _gwAddModelCommand(gwc)
{
    
}

GWAddModelCommand3dView::~GWAddModelCommand3dView()
{

}
