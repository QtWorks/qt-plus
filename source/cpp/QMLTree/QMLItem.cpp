
// Application
#include "QMLItem.h"

//-------------------------------------------------------------------------------------------------

/*!
    \class QMLItem
    \inmodule qt-plus
    \brief The base item for a QML tree.
*/

//-------------------------------------------------------------------------------------------------

/*!
    Constructs a QMLItem.
*/
QMLItem::QMLItem()
{
}

//-------------------------------------------------------------------------------------------------

/*!
    Constructs a QMLItem with \a value.
*/
QMLItem::QMLItem(const QVariant& value)
    : m_vValue(value)
{
}

//-------------------------------------------------------------------------------------------------

/*!
    Destroys a QMLItem.
*/
QMLItem::~QMLItem()
{
}

//-------------------------------------------------------------------------------------------------

/*!
    Sets the item's value to \a value.
*/
void QMLItem::setValue(const QVariant& value)
{
    m_vValue = value;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's value.
*/
QVariant QMLItem::value() const
{
    return m_vValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Dumps the item to \a stream using \a iIdent for indentation.
*/
void QMLItem::dump(QTextStream& stream, int iIdent)
{
    QString sValue = m_vValue.value<QString>();

    if (sValue.isEmpty() == false)
    {
        dumpIndented(stream, iIdent, QString("[QMLItem]"));
        dumpIndented(stream, iIdent, QString("Value : %1").arg(sValue));
    }

    dumpIndented(stream, iIdent, QString("----------"));
}

//-------------------------------------------------------------------------------------------------

/*!
    Dumps the item to \a stream using \a iIdent for indentation. \br\br
    \a pContext is the context of this item. \br
    \a pParent is the caller of this method.
*/
void QMLItem::toQML(QTextStream& stream, QMLTreeContext* pContext, QMLItem* pParent, int iIdent)
{
    Q_UNUSED(pContext);
    Q_UNUSED(pParent);

    QString sValue;

    switch (m_vValue.type())
    {
        case QVariant::Int :
        case QVariant::Double :
            sValue = m_vValue.toString();
            break;

        default :
            sValue = QString("\"%1\"").arg(m_vValue.toString());
    }

    dumpNoIndentNoNewLine(stream, sValue);
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a CXMLNode representation of this item. \br\br
    \a pContext is a user defined context. \br
    \a pParent is the caller of this method.
*/
CXMLNode QMLItem::toXMLNode(CXMLNodableContext* pContext, CXMLNodable* pParent)
{
    CXMLNode xNode(metaObject()->className());
    QString sValue = m_vValue.value<QString>();

    if (sValue.isEmpty() == false)
    {
        xNode.attributes()["Value"] = sValue;
    }

    return xNode;
}
