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

GWModel3dView::GWModel3dView(const GWModel& gwm, GW3dView& gw3dv)
    : _gwModel(gwm)
    , _gw3dView(gw3dv)
{

}

GWModel3dView::~GWModel3dView()
{

}

void GWModel3dView::loadModelToScene()
{
    //get name of the model we're supposed to be reading
    const std::string file = _gwModel.getUrl().toStdString();

    //read the file
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(file);
    
    //make a transform based on the current scene bounding sphere
    osg::ref_ptr<osg::Group> root = _gw3dView.getRoot();
    float radius = root->getBound().radius();
    osg::Matrixf mat = osg::Matrixf::translate(radius, 0.f, 0.f);
    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform(mat);
    
    trans->addChild(loadedModel);
    root->addChild(trans);
}

void GWModel3dView::removeModelFromScene()
{
    osg::ref_ptr<osg::Group> root = _gw3dView.getRoot();
    root->removeChild(root->getNumChildren()-1);
}
