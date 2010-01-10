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

GWCommand3dView::GWCommand3dView()
{

}

GWCommand3dView::~GWCommand3dView()
{

}

GWAddModelCommand3dView::GWAddModelCommand3dView(const GWAddModelCommand& gwc)
    : _gwAddModelCommand(gwc)
{
    
}

GWAddModelCommand3dView::~GWAddModelCommand3dView()
{

}

void GWAddModelCommand3dView::update3dView(GW3dView& gw3dv) const
{
    const GWModel& gwm = _gwAddModelCommand.getGWModel();

    GWModel3dView gwm3dv(gwm,gw3dv);

    if(_gwAddModelCommand.isRedoing())
    {
        gwm3dv.loadModelToScene();
    }
    else // undoing
    {
        gwm3dv.removeModelFromScene();
    }
}

