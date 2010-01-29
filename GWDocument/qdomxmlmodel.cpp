#include <QtCore/QVector>
#include <QtCore/QtDebug>

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaProperty>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>

#include "qdomxmlmodel.h"

QT_BEGIN_NAMESPACE

//static convenience functions for performing select type operations with an
//xpath statement on a node and getting results as a QDomNodeList
QDomNode QDomXmlModel::selectNode(const QDomNode& qdn, const QString& xpath)
{
    QList<QDomNode> nodeList = selectNodes(qdn, xpath, true);
    QMutableListIterator<QDomNode> itr(nodeList);
    
    if(itr.hasNext())
    {
        return itr.next();
    }

    return QDomNode();
}

QList<QDomNode> QDomXmlModel::selectNodes(const QDomNode& qdn, const QString& xpath, bool firstNodeOnly)
{
    QList<QDomNode> nodeList;

    //populate it
    QXmlNamePool namePool;
    QDomXmlModel qDomModel(qdn.ownerDocument(), namePool);
    QXmlQuery query(namePool);

    query.setFocus(qDomModel.makeIndex(qdn));
    query.setQuery(xpath);

    QXmlResultItems result;

    if (query.isValid())
    {
        query.evaluateTo(&result);
        QXmlItem item(result.next());
        while (!item.isNull())
        {
            QXmlNodeModelIndex n = item.toNodeModelIndex();
            QDomNode domNode = qDomModel.toDomNode(n);
            nodeList.append(domNode);

            if(firstNodeOnly) //if you're only interested in the first node we find
            {
                break;
            }
            
            item = result.next();
        }

        if (result.hasError())
        {
            //what to do with this error?
            ;
        }
    }

    return nodeList;
}

QDomXmlModel::QDomXmlModel(QDomDocument domDoc, const QXmlNamePool &np)
    : QSimpleXmlNodeModel(np),
      m_baseURI(QUrl::fromLocalFile(QCoreApplication::applicationFilePath())),
      m_doc(domDoc)
{
    Q_ASSERT(m_baseURI.isValid());
}

QXmlNodeModelIndex::DocumentOrder QDomXmlModel::compareOrder(const QXmlNodeModelIndex& n1, const QXmlNodeModelIndex& n2) const
{
    //could do something here like find the last common ancestor and examine its child order

    //dummy implementation for now
    Q_UNUSED(n1);
    Q_UNUSED(n2);
    return QXmlNodeModelIndex::Follows;
}

QXmlName QDomXmlModel::name(const QXmlNodeModelIndex &n) const
{
    /*
    From the api documentation:

    Returns the name of ni. The caller guarantees that ni is not null and that it belongs to this
    QAbstractXmlNodeModel.

    If a node does not have a name, e.g., comment nodes, a null QXmlName is returned. QXmlNames
    must be created with the instance of QXmlQuery that is being used for evaluating queries using
    this QAbstractXmlNodeModel.

    This function maps to the dm:node-name() accessor.

    If ni is a processing instruction, a QXmlName is returned with the local name as the target name
    and the namespace URI and prefix both empty.
    */

    QDomNode domNode = toDomNode(n);

    switch (domNode.nodeType())
    {
        //named nodes
        case QDomNode::ElementNode:
        case QDomNode::AttributeNode:
        case QDomNode::EntityReferenceNode:
        case QDomNode::EntityNode:
        case QDomNode::DocumentNode:
        case QDomNode::DocumentFragmentNode:
        case QDomNode::DocumentTypeNode:
           return QXmlName(namePool(), domNode.nodeName(), domNode.namespaceURI(), domNode.prefix());

        //specials
        case QDomNode::ProcessingInstructionNode:
            return QXmlName(namePool(), domNode.localName());

        //unnamed nodes
        case QDomNode::TextNode:
        case QDomNode::CDATASectionNode:
        case QDomNode::CommentNode:
        default:
            return QXmlName();
    }

    Q_ASSERT(false);
    return QXmlName();
}

QUrl QDomXmlModel::documentUri(const QXmlNodeModelIndex& ) const
{
    return m_baseURI;
}

QXmlNodeModelIndex::NodeKind QDomXmlModel::kind(const QXmlNodeModelIndex& n) const
{
    QDomNode domNode = toDomNode(n);

    if(!domNode.isNull())
    {
        switch (domNode.nodeType())
        {
            //named nodes
            case QDomNode::ElementNode:
            case QDomNode::EntityReferenceNode:
            case QDomNode::EntityNode:
            case QDomNode::DocumentFragmentNode:
                return QXmlNodeModelIndex::Element;
            case QDomNode::AttributeNode:
                return QXmlNodeModelIndex::Attribute;
            case QDomNode::DocumentNode:
                return QXmlNodeModelIndex::Document;
            case QDomNode::TextNode:
                return QXmlNodeModelIndex::Text;
            case QDomNode::CommentNode:
            case QDomNode::CDATASectionNode:
            case QDomNode::DocumentTypeNode:
                return QXmlNodeModelIndex::Comment;
            case QDomNode::ProcessingInstructionNode:
                return QXmlNodeModelIndex::ProcessingInstruction;
            default:
                return QXmlNodeModelIndex::Element;
        }
    }

    Q_ASSERT(false);
    return QXmlNodeModelIndex::Element;
}

class QDomNodeIdExtractor : public QDomNode
{
public:
    QDomNodeIdExtractor(QDomNode qdm) : QDomNode(qdm) {}
    qint64 extractId()
    {
        return qint64(this->impl);
    }
};

QDomNode QDomXmlModel::toDomNode(const QXmlNodeModelIndex& n) const
{
    qint64 attrId = n.additionalData();
    if(attrId)
    {
        QMap<qint64,QDomNode>::const_iterator citr = m_nodeIdMap.find(attrId);
        if(citr != m_nodeIdMap.end())
        {
            QDomNode attrNode = *citr;
            return attrNode;
        }
    }
    else
    {
        qint64 elemId = n.data();
        if(elemId)
        {
            QMap<qint64,QDomNode>::const_iterator citr = m_nodeIdMap.find(elemId);
            if(citr != m_nodeIdMap.end())
            {
                QDomNode elemNode = *citr;
                return elemNode;
            }
        }
    }

    Q_ASSERT(false);
    return QDomNode();
}

qint64 QDomXmlModel::getDomNodeId(QDomNode node) const
{
    QDomNodeIdExtractor idExtractor = node;
    qint64 id = idExtractor.extractId();
    if(id)
    {
        if(!m_nodeIdMap.contains(id))
        {
            m_nodeIdMap[id] = node;
        }
        return id;
    }

    return 0;
}

QXmlNodeModelIndex QDomXmlModel::doc() const
{
    if(!m_doc.isNull())
    {
        return makeIndex(m_doc);
    }

    Q_ASSERT(false);
    return QXmlNodeModelIndex();
}

/**
 * does this return the document?
 */
QXmlNodeModelIndex QDomXmlModel::root(const QXmlNodeModelIndex& n) const
{
    Q_UNUSED(n);
    return doc();
}

/*
  From the api documentation:

  QDomAttr                    The attribute value
  QDomCDATASection            The content of the CDATA section
  QDomComment                 The comment
  QDomProcessingInstruction   The data of the processing instruction
  QDomText                    The text
*/
QString QDomXmlModel::stringValue(const QXmlNodeModelIndex &n) const
{
    QDomNode domNode = toDomNode(n);

    if(!domNode.isNull())
    {
        switch (domNode.nodeType())
        {
            //case QDomNode::AttributeNode:
            case QDomNode::CDATASectionNode:
            case QDomNode::CommentNode:
            case QDomNode::ProcessingInstructionNode:
            case QDomNode::TextNode:
            {
                QString domTxtNodeValue = domNode.nodeValue();
                return domTxtNodeValue;
            }
            default:
                return QSimpleXmlNodeModel::stringValue(n);
        }
    }

    return QSimpleXmlNodeModel::stringValue(n);
}

QVariant QDomXmlModel::typedValue(const QXmlNodeModelIndex &n) const
{
    QDomNode domNode = toDomNode(n);

    if(!domNode.isNull())
    {
        switch (domNode.nodeType())
        {
        case QDomNode::ElementNode:
            {
                QDomNode firstChild = domNode.firstChild();
                if(firstChild.isText())
                {
                    return QVariant(firstChild.nodeValue());
                }
             }
        case QDomNode::AttributeNode:
                return QVariant(domNode.nodeValue());
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QXmlNodeModelIndex QDomXmlModel::makeIndex(QDomNode qdm) const
{
    if(!qdm.isNull())
    {
        if(qdm.isAttr())
        {
            qint64 elemId = getDomNodeId(qdm.parentNode());
            qint64 attrId = getDomNodeId(qdm);

            if(elemId && attrId)
            {
                return createIndex(elemId,attrId);
            }
        }
        else
        {
            qint64 elemId = getDomNodeId(qdm);

            if(elemId)
            {
                return createIndex(elemId);
            }
        }
    }

    return QXmlNodeModelIndex();
}

QVector<QXmlNodeModelIndex> QDomXmlModel::attributes(const QXmlNodeModelIndex& n) const
{
    QDomNode domNode = toDomNode(n);

    QVector<QXmlNodeModelIndex> result;

    if(!domNode.isNull() && domNode.isElement())
    {
        QDomElement elem = domNode.toElement();
        Q_ASSERT(!elem.isNull());

        QDomNamedNodeMap children = domNode.attributes();

        for(int i=0; i<children.count(); ++i)
        {
            QDomNode child = children.item(i);

            if(!child.isNull() && child.isAttr())
            {
                result.append(makeIndex(child));
            }
        }
    }

    return result;
}

QXmlNodeModelIndex QDomXmlModel::nextFromSimpleAxis(SimpleAxis axis, const QXmlNodeModelIndex &n) const
{
    /**
     * QSimpleXmlNodeModel eliminates the need to handle redundant corner cases by guaranteeing that it
     * will never ask for:
     *
     * Children or siblings for attributes.
     * Children for comments, processing instructions, and text nodes.
     * Siblings or parents for document nodes.
     */

    QDomNode domNode = toDomNode(n);

    if(!domNode.isNull())
    {
        switch (axis)
        {
            case Parent:
            {
                QDomNode parent = domNode.parentNode();
                return makeIndex(parent);
            }
            case PreviousSibling:
            {
                QDomNode prevSib = domNode.previousSibling();
                return makeIndex(prevSib);
            }
            case NextSibling:
            {
                QDomNode nextSib = domNode.nextSibling();
                return makeIndex(nextSib);
            }
            case FirstChild:
            {
                QDomNode firstChild = domNode.firstChild();
                return makeIndex(firstChild);
            }
        }
    }

    Q_ASSERT(false);
        return QXmlNodeModelIndex();
}

//possible other overrides - at present these are straight copies of what's in QSimpleXmlNodeModel

//if I discover I need namespace support, I'll have to implement this
QVector<QXmlName> QDomXmlModel::namespaceBindings(const QXmlNodeModelIndex &node) const
{
    QDomNode domNode = toDomNode(node);

    if(!domNode.isNull())
    {
        domNode.namespaceURI();

    }

    return QVector<QXmlName>();
}

//if I discover I need id/idref support, i'll have to implement the following two functions properly
QXmlNodeModelIndex QDomXmlModel::elementById(const QXmlName &id) const
{
    Q_UNUSED(id);
    return QXmlNodeModelIndex();
}

QVector<QXmlNodeModelIndex> QDomXmlModel::nodesByIdref(const QXmlName &idref) const
{
    Q_UNUSED(idref);
    return QVector<QXmlNodeModelIndex>();
}

QT_END_NAMESPACE
