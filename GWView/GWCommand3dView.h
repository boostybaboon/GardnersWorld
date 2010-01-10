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

#ifndef GWCOMMAND3DVIEW_H
#define GWCOMMAND3DVIEW_H

class GWCommand;
class GWAddModelCommand;
class GW3dView;

class GWCommand3dView
{
public:
    GWCommand3dView();
    virtual ~GWCommand3dView();

    virtual void update3dView(GW3dView& gw3dv) const = 0;
};

class GWAddModelCommand3dView : public GWCommand3dView
{
public:
    GWAddModelCommand3dView(const GWAddModelCommand& gwc);
    virtual ~GWAddModelCommand3dView();

    virtual void update3dView(GW3dView& gw3dv) const;

private:
    const GWAddModelCommand& _gwAddModelCommand;
};

#endif // GWCOMMAND3DVIEW_H
