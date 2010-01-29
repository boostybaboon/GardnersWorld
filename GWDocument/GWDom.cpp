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

#include "GWDom.h"

#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtCore/QObject>

#include <iostream>

GWDom::GWDom()
    : _doc("SomeName")

{

}

GWDom::~GWDom()
{

}

QDomDocument GWDom::createDOMDocumentFromFile(const QString& fileName)
{
    if (fileName.isEmpty())
    {
        return _doc;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(NULL, QObject::tr("Dom node model"), QObject::tr("Cannot read file %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return _doc;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!_doc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox::information(NULL, QObject::tr("DOM node model"),
                                 QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
        return _doc;
    }

    return _doc;
}

QDomDocument GWDom::newDomDocument(const QString& docName)
{
    QDomElement root = _doc.createElement(docName);
    _doc.appendChild(root);

    return _doc;
}

QDomElement GWDom::getDocumentElement()
{
    return _doc.documentElement();
}

QDomElement GWDom::newElement(const QString& elementName)
{
    QDomElement elem = _doc.createElement(elementName);
    return elem;
}

QDomAttr GWDom::newAttribute(const QString& attrName)
{
    QDomAttr attr = _doc.createAttribute(attrName);
    return attr;
}

