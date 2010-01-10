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

#ifndef GWVIEW_H
#define GWVIEW_H

#include "../GWDocument/GWDocument.h"

#include <osg/Group>

#include <string>

class GWObserver : public QObject
{
    Q_OBJECT
    
public:
    GWObserver(const GWSubject& s)
        : _subject(s)
    {
        _subject.registerObserver(*this);
    }

    virtual ~GWObserver()
    {
        _subject.unregisterObserver(*this);
    }

public slots:
    virtual void update(const GWCommand& m) = 0;
    
private:
    const GWSubject& _subject;
};

class GWView : public GWObserver
{
public:
    GWView(const GWDocument& d);
    virtual ~GWView();

protected:
    const GWDocument& _gwDocument;
};

namespace osgViewer
{
    class Viewer;
    class GraphicsWindow;
}

namespace osg
{
    class Node;
}

class GW3dView : public GWView
{
public:
    GW3dView(const GWDocument& d);
    virtual ~GW3dView();
    
    void initialise(const int winId, const int x, const int y, const int width, const int height);

    void closeWindow();
    void windowResize(const int width, const int height);
    void keyPress(const int c);
    void keyRelease(const int c);
    void mouseButtonPress(const int x, const int y, const int button);
    void mouseButtonRelease(const int x, const int y, const int button);
    void mouseDoubleButtonPress(const int x, const int y, const int button);
    void mouseMotion(const int x, const int y);

    virtual void update(const GWCommand& m);

    void tick();
    
    void testAddModelFileToRootNode(const std::string& file);

    osg::ref_ptr<osg::Group> getRoot();

private:
    void setManipulatorAndEventHandlers();
    
    osg::ref_ptr<osgViewer::Viewer> _osgViewer;
    osg::ref_ptr<osgViewer::GraphicsWindow> _osgGraphicsWindow;
    osg::ref_ptr<osg::Group>  _root;
};

#endif // GWVIEW_H
