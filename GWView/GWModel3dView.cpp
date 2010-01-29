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

#include "GWModel3dView.h"

#include "../GWDocument/GWNode/GWModel.h"
#include "GWView.h"

#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

GWNode3dView::GWNode3dView(GW3dView& gw3dv, const GWNode& gwn)
    : _gw3dView(gw3dv)
    , _gwNode(gwn)
{

}

GWNode3dView::~GWNode3dView()
{

}

void GWNode3dView::removeFrom3dView(GWSceneGraphPosition preCommandPos)
{
    removeSceneGraphNode(preCommandPos);
    postRemoveSceneGraphNode();
}

/**
 * If the node exists in the scene graph, remove it from its parent
 */
void GWNode3dView::removeSceneGraphNode(GWSceneGraphPosition preCommandPos)
{
    QMap<int,osg::ref_ptr<osg::Node> >& nodeMap =_gw3dView.getNodeMap();

    const int modelId = _gwNode.getGlobalId();

    if(nodeMap.contains(modelId))
    {
        osg::ref_ptr<osg::Node> modelNode = nodeMap.value(modelId);

        //should be able to identify a parent if we get to here
        if(nodeMap.contains(preCommandPos.parent()))
        {
            osg::ref_ptr<osg::Node> parentNode = nodeMap.value(preCommandPos.parent());
            if(modelNode.valid() && parentNode.valid())
            {
                osg::Group* parentNodeAsGroup = parentNode->asGroup();
                if(parentNodeAsGroup)
                {
                    parentNodeAsGroup->removeChild(modelNode);
                    nodeMap.remove(modelId);
                }
            }
        }
    }
}

void GWNode3dView::addTo3dView()
{
    osg::ref_ptr<osg::Node> osgNode = makeSceneGraphNode();
    addSceneGraphNode(osgNode);
    postAddSceneGraphNode();
}

void GWNode3dView::addSceneGraphNode(osg::ref_ptr<osg::Node> osgNode)
{
    GWSceneGraphPosition pos = _gwNode.getSceneGraphPosition();

    QMap<int,osg::ref_ptr<osg::Node> >& nodeMap =_gw3dView.getNodeMap();

    const int modelId = _gwNode.getGlobalId();

    int parentId = pos.parent();

    if(parentId > -1 && nodeMap.contains(parentId))
    {
        osg::ref_ptr<osg::Node> parentNode = nodeMap.value(parentId);
        if(parentNode.valid())
        {
            osg::Group* parentNodeAsGroup = parentNode->asGroup();
            if(parentNodeAsGroup)
            {
                int nextSiblingId = pos.nextSibling();

                if(nextSiblingId > -1 && nodeMap.contains(nextSiblingId))
                {
                    osg::ref_ptr<osg::Node> nextSiblingNode = nodeMap.value(nextSiblingId);
                    if(nextSiblingNode.valid())
                    {
                        uint nextSiblingIdx = parentNodeAsGroup->getChildIndex(nextSiblingNode);
                        parentNodeAsGroup->insertChild(nextSiblingIdx,osgNode);
                        nodeMap.insert(modelId,osgNode);
                    }
                }
                else
                {
                    parentNodeAsGroup->addChild(osgNode);
                    nodeMap.insert(modelId,osgNode);
                }
            }
        }
    }
}

GWModel3dView::GWModel3dView(GW3dView& gw3dv, const GWModel& gwm)
    : GWNode3dView(gw3dv, gwm)
    , _gwModel(gwm)
{

}

GWModel3dView::~GWModel3dView()
{

}

osg::ref_ptr<osg::Node> GWModel3dView::makeSceneGraphNode() const
{
    //get name of the model we're supposed to be reading
    const std::string file = _gwModel.getUrl().toStdString();

    //read the file
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(file);

    return loadedModel;

    //make a transform (not doing anything with it at the moment)
    //float radius = root->getBound().radius();
    //osg::Matrixf mat; // = osg::Matrixf::translate(radius, 0.f, 0.f);
    //osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform(mat);

    //trans->addChild(loadedModel);

    //return trans;
}
