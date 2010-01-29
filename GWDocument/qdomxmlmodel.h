#ifndef _QDomNodeModel_H
#define _QDomNodeModel_H

#include <QtXmlPatterns/QSimpleXmlNodeModel>
#include <QtXml/QDomDocument>
#include <QtCore/QMap>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QDomXmlModel : public QSimpleXmlNodeModel
{
  public:
    QDomXmlModel(QDomDocument doc, const QXmlNamePool &np);

    QXmlNodeModelIndex doc() const;

    virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex &n1, const QXmlNodeModelIndex &n2) const;
    virtual QXmlName name(const QXmlNodeModelIndex &n) const;
    virtual QUrl documentUri(const QXmlNodeModelIndex &n) const;
    virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &n) const;
    virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &n) const;
    virtual QVariant typedValue(const QXmlNodeModelIndex &n) const;
    virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex&) const;
    virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis, const QXmlNodeModelIndex&) const;

    //possible other overrides...
    QString stringValue(const QXmlNodeModelIndex &n) const;
    QVector<QXmlName> namespaceBindings(const QXmlNodeModelIndex &node) const;
    QXmlNodeModelIndex elementById(const QXmlName &id) const;
    QVector<QXmlNodeModelIndex> nodesByIdref(const QXmlName &idref) const;

    QDomNode toDomNode(const QXmlNodeModelIndex& n) const;
    QXmlNodeModelIndex makeIndex(QDomNode qdm) const;
    
    static QDomNode selectNode(const QDomNode& qdn, const QString& xpath);
    static QList<QDomNode> selectNodes(const QDomNode& qdn, const QString& xpath, bool firstNodeOnly = false);
    
  private:
    qint64 getDomNodeId(QDomNode node) const;

    const QUrl m_baseURI;
    QDomDocument m_doc;
    mutable QMap<qint64,QDomNode> m_nodeIdMap;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
