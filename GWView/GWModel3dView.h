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

#ifndef GWMODEL3DVIEW_H
#define GWMODEL3DVIEW_H

#include "GWView.h"

class GWNode;
class GWModel;

class GWNode3dView
{
public:
    GWNode3dView(GW3dView& gw3dv, const GWNode& gwn);
    virtual ~GWNode3dView();

    void removeFrom3dView(GWSceneGraphPosition preCommandPos);
    void addTo3dView();

protected:
    virtual void removeSceneGraphNode(GWSceneGraphPosition preCommandPos);
    virtual void postRemoveSceneGraphNode() {}

    virtual osg::ref_ptr<osg::Node> makeSceneGraphNode() const = 0;
    virtual void addSceneGraphNode(osg::ref_ptr<osg::Node> node);
    virtual void postAddSceneGraphNode() {}

    GW3dView& _gw3dView;

private:
    const GWNode& _gwNode;
};

class GWModel3dView : public GWNode3dView
{
public:
    GWModel3dView(GW3dView& gw3dv, const GWModel& gwm);
    virtual ~GWModel3dView();

    osg::ref_ptr<osg::Node> makeSceneGraphNode() const;
    
private:
    const GWModel& _gwModel;
};

#endif // GWMODEL3DVIEW_H
