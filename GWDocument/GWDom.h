/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef GWDOM_H
#define GWDOM_H

#include <QtXml/QDomDocument>

#include <iostream>

class GWDom
{
public:
    GWDom();
    virtual ~GWDom();
    
    QDomDocument createDOMDocumentFromFile(const QString& fileName);
    QDomDocument newDomDocument(const QString& docName);
    QDomElement getDocumentElement();
    QDomElement newElement(const QString& elementName);
    QDomAttr newAttribute(const QString& attrName);
    
private:
    QDomDocument _doc;
};

#endif // GWDOM_H
