project(GardnersWorld)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_MODULE_PATH "${GardnersWorld_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

find_package(Qt4 REQUIRED)
find_package(OpenSceneGraph 2.6.0 COMPONENTS osg osgGA osgViewer)

set(GARDNERSWORLD_SRCS
#   Ui bits
    GWUserInterface/main.cpp
    GWUserInterface/GWMainWindow.cpp
    GWUserInterface/GW3dViewWidget.cpp

#   Controller bits
    GWController/GWController.cpp
    GWController/GWCommand.cpp
    
#   Document bits
    GWDocument/GWDocument.cpp
    GWDocument/GWDom.cpp
#    GWDocument/GWCommandStack.cpp
#    GWDocument/GWX3DNode.cpp
    GWDocument/GWNode/GWModel.cpp
    GWDocument/qdomxmlmodel.cpp

#   View bits
    GWView/GWView.cpp
    GWView/GWCommand3dView.cpp
    GWView/GWModel3dView.cpp
)

SET(GARDNERSWORLD_RCS GWUserInterface/GardnersWorld.qrc)
QT4_ADD_RESOURCES(GARDNERSWORLD_RCS_SRCS ${GARDNERSWORLD_RCS})

set(GARDNERSWORLD_UIS
    GWUserInterface/GWMainWindowUi.ui
)

QT4_WRAP_UI(GARDNERSWORLD_UIS_H
    ${GARDNERSWORLD_UIS}
)

set(GARDNERSWORLD_MOC_HDRS
    GWUserInterface/GWMainWindow.h
    GWDocument/GWDocument.h
    GWView/GWView.h
)

#do i have to #if this for linux/windows/mac?
add_definitions(-W -Wall -Wshadow -Werror -Wno-error=unused-parameter)

QT4_WRAP_CPP(GARDNERSWORLD_MOC_SRCS
    ${GARDNERSWORLD_MOC_HDRS}
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

add_executable(GWBrowser
    ${GARDNERSWORLD_SRCS} 
    ${GARDNERSWORLD_UIS_H}
    ${GARDNERSWORLD_MOC_SRCS}
    ${GARDNERSWORLD_RCS_SRCS}
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_link_libraries(GWBrowser
    ${QT_QTCORE_LIBRARY_DEBUG}
    ${QT_QTGUI_LIBRARY_DEBUG}
    ${QT_QTXML_LIBRARY_DEBUG}
    ${QT_QTXMLPATTERNS_LIBRARY_DEBUG}
    ${OPENSCENEGRAPH_LIBRARIES}
  )
else()
  target_link_libraries(GWBrowser
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${OPENSCENEGRAPH_LIBRARIES}
  )
endif()
