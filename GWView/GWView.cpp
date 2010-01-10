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

#include "GWView.h"
#include "GWCommand3dView.h"

#include <QtCore/QSharedPointer>

GWView::GWView(const GWDocument& d)
    : GWObserver(d)
    , _gwDocument(d)
{
    
}

GWView::~GWView()
{
    
}

#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgDB/ReadFile>

#if defined(WIN32) && !defined(__CYGWIN__)
#include <osgViewer/api/Win32/GraphicsWindowWin32>
typedef HWND WindowHandle;
typedef osgViewer::GraphicsWindowWin32::WindowData WindowData;
#elif defined(__APPLE__) && defined(APPLE_PRE_10_3)
#include <osgViewer/api/Carbon/GraphicsWindowCarbon>
typedef WindowRef WindowHandle;
typedef osgViewer::GraphicsWindowCarbon::WindowData WindowData;
#else // all other unix
#include <osgViewer/api/X11/GraphicsWindowX11>
typedef Window WindowHandle;
typedef osgViewer::GraphicsWindowX11::WindowData WindowData;
#endif

GW3dView::GW3dView(const GWDocument& d)
    : GWView(d)
    , _osgViewer(0)
    , _osgGraphicsWindow(0)
    , _root(new osg::Group)
{

}

GW3dView::~GW3dView()
{

}

void GW3dView::initialise(const int winId, const int x, const int y, const int width, const int height)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();
    
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    
    traits->readDISPLAY();
    if(traits->displayNum<0)
    {
        traits->displayNum = 0;
    }
    
    traits->windowName = "GWView";
    traits->screenNum = 0;
    traits->x = x;
    traits->y = y;
    traits->width = width;
    traits->height = height;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->windowDecoration = false;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
    
    traits->inheritedWindowData = new WindowData(winId);
    
    if(ds->getStereo())
    {
        switch(ds->getStereoMode())
        {
            case(osg::DisplaySettings::QUAD_BUFFER) :
                traits->quadBufferStereo = true;
                break;
            case(osg::DisplaySettings::VERTICAL_INTERLACE) :
            case(osg::DisplaySettings::CHECKERBOARD) :
            case(osg::DisplaySettings::HORIZONTAL_INTERLACE) :
                traits->stencil = 8;
                break;
            default:
                break;
        }
    }
    
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    _osgGraphicsWindow = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
    
    _osgViewer = new osgViewer::Viewer;
    _osgViewer->getCamera()->setViewport(0, 0, width, height);
    _osgViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height), 1.0f, 10000.0f);
    _osgViewer->getCamera()->setGraphicsContext(_osgGraphicsWindow);

    //this is the bit which gets the viewer to start viewing the model (nothing in it yet)
    _osgViewer->setSceneData (_root);
    
    //set some manipulators to do stuff to the scene
    setManipulatorAndEventHandlers();
}

void GW3dView::closeWindow()
{
    _osgGraphicsWindow->getEventQueue()->closeWindow();
}

void GW3dView::windowResize(const int width, const int height)
{
    _osgGraphicsWindow->getEventQueue()->windowResize(0, 0, width, height);
    _osgGraphicsWindow->resized(0, 0, width, height);
    _osgViewer->getCamera()->setViewport(0, 0, width, height);
    _osgViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height), 1.0f, 10000.0f);
}

void GW3dView::keyPress(const int c)
{
    _osgGraphicsWindow->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)(c));
}

void GW3dView::keyRelease(const int c)
{
    _osgGraphicsWindow->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)(c));
}

void GW3dView::mouseButtonPress(const int x, const int y, const int button)
{
    _osgGraphicsWindow->getEventQueue()->mouseButtonPress(x, y, button);
}

void GW3dView::mouseButtonRelease(const int x, const int y, const int button)
{
    _osgGraphicsWindow->getEventQueue()->mouseButtonRelease(x, y, button);
}

void GW3dView::mouseDoubleButtonPress(const int x, const int y, const int button)
{
    _osgGraphicsWindow->getEventQueue()->mouseDoubleButtonPress(x, y, button);
}

void GW3dView::mouseMotion(const int x, const int y)
{
    _osgGraphicsWindow->getEventQueue()->mouseMotion(x, y);
}

void GW3dView::update(const GWCommand& gwc)
{
    QSharedPointer<GWCommand3dView> cmd3dView(gwc.getCommand3dView());
    cmd3dView->update3dView(*this);
}

void GW3dView::setManipulatorAndEventHandlers()
{
    //set a trackball manipulator
    _osgViewer->setCameraManipulator(new osgGA::TrackballManipulator());
    
    // add the state manipulator
    _osgViewer->addEventHandler(new osgGA::StateSetManipulator(_osgViewer->getCamera()->getOrCreateStateSet()));
    
    // add the thread model handler
    _osgViewer->addEventHandler (new osgViewer::ThreadingHandler);
    
    // add the window size toggle handler
    //viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    
    // add the stats handler
    _osgViewer->addEventHandler(new osgViewer::StatsHandler);
    
    // add the help handler
    //viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));
    _osgViewer->addEventHandler(new osgViewer::HelpHandler);
}

void GW3dView::tick()
{
    _osgViewer->frame();
}

osg::ref_ptr<osg::Group> GW3dView::getRoot()
{
    return _root;
}
