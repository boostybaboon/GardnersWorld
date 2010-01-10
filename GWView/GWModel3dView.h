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

class GWModel;
class GW3dView;

class GWModel3dView
{
public:
    GWModel3dView(const GWModel& gwm, GW3dView& gw3dv);
    virtual ~GWModel3dView();

    virtual void loadModelToScene();
    virtual void removeModelFromScene();
    
private:
    const GWModel& _gwModel;
    GW3dView& _gw3dView;
};

#endif // GWMODEL3DVIEW_H
