
// Application
#include "QMLComplexEntity.h"

//-------------------------------------------------------------------------------------------------

QMLComplexEntity::QMLComplexEntity(const QPoint& pPosition, QMLEntity* pName)
    : QMLEntity(pPosition)
    , m_pName(pName)
    , m_bIsArray(false)
    , m_bIsObject(false)
    , m_bIsBlock(false)
    , m_bIsArgumentList(false)
{
}

//-------------------------------------------------------------------------------------------------

QMLComplexEntity::~QMLComplexEntity()
{
    if (m_pName != nullptr)
    {
        delete m_pName;
    }

    foreach (QMLEntity* pItem, m_vContents)
    {
        if (pItem != nullptr)
        {
            delete pItem;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::setName(QMLEntity* pName)
{
    if (m_pName != nullptr)
        delete m_pName;

    m_pName = pName;
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::setIsArray(bool bValue)
{
    m_bIsArray = bValue;
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::setIsObject(bool bValue)
{
    m_bIsObject = bValue;
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::setIsBlock(bool bValue)
{
    m_bIsBlock = bValue;
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::setIsArgumentList(bool bValue)
{
    m_bIsArgumentList = bValue;
}

//-------------------------------------------------------------------------------------------------

QMLEntity* QMLComplexEntity::name() const
{
    return m_pName;
}

//-------------------------------------------------------------------------------------------------

QVector<QMLEntity*>& QMLComplexEntity::contents()
{
    return m_vContents;
}

//-------------------------------------------------------------------------------------------------

bool QMLComplexEntity::isNamed() const
{
    return (m_pName != nullptr && m_pName->value().toString().isEmpty() == false);
}

//-------------------------------------------------------------------------------------------------

bool QMLComplexEntity::isArray() const
{
    return m_bIsArray;
}

//-------------------------------------------------------------------------------------------------

bool QMLComplexEntity::isObject() const
{
    return m_bIsObject;
}

//-------------------------------------------------------------------------------------------------

bool QMLComplexEntity::isBlock() const
{
    return m_bIsBlock;
}

//-------------------------------------------------------------------------------------------------

bool QMLComplexEntity::isArgumentList() const
{
    return m_bIsArgumentList;
}

//-------------------------------------------------------------------------------------------------

const QVector<QMLEntity*>& QMLComplexEntity::contents() const
{
    return m_vContents;
}

//-------------------------------------------------------------------------------------------------

QString QMLComplexEntity::toString() const
{
    QString sReturnValue;

    foreach (QMLEntity* pItem, m_vContents)
    {
        if (pItem != nullptr)
        {
            if (sReturnValue.isEmpty())
            {
                sReturnValue += ", ";
            }

            sReturnValue += pItem->toString();
        }
    }

    return sReturnValue;
}

//-------------------------------------------------------------------------------------------------

QMap<QString, QMLEntity*> QMLComplexEntity::members()
{
    QMap<QString, QMLEntity*> vReturnValue;

    vReturnValue["name"] = m_pName;

    return vReturnValue;
}

//-------------------------------------------------------------------------------------------------

QVector<QMLEntity*> QMLComplexEntity::grabContents()
{
    QVector<QMLEntity*> vReturnValue = m_vContents;
    m_vContents.clear();
    return vReturnValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Finds the origin of the item. \br\br
    \a pContext is the context of this item. \br
*/
void QMLComplexEntity::solveOrigins(QMLTreeContext* pContext)
{
    QMLEntity::solveOrigins(pContext);

    foreach (QMLEntity* pItem, m_vContents)
    {
        pItem->setParent(this);
        pItem->solveOrigins(pContext);
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a list of all declared symbols.
*/
QMap<QString, QMLEntity*> QMLComplexEntity::getDeclaredSymbols()
{
    QMap<QString, QMLEntity*> mReturnValue = QMLEntity::getDeclaredSymbols();

    foreach (QMLEntity* pItem, m_vContents)
    {
        if (pItem != nullptr)
        {
            QMap<QString, QMLEntity*> itemSymbols = pItem->getDeclaredSymbols();

            foreach (QString sKey, itemSymbols.keys())
            {
                if (mReturnValue.contains(sKey) == false)
                {
                    mReturnValue[sKey] = itemSymbols[sKey];
                }
            }
        }
    }

    return mReturnValue;
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::checkSymbolUsages(QMLTreeContext* pContext)
{
    QMLEntity::checkSymbolUsages(pContext);

    foreach (QMLEntity* pItem, m_vContents)
    {
        if (pItem != nullptr)
        {
            pItem->checkSymbolUsages(pContext);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void QMLComplexEntity::toQML(QTextStream& stream, QMLTreeContext* pContext, QMLEntity* pParent, int iIdent)
{
    Q_UNUSED(pContext);
    Q_UNUSED(pParent);

    if (m_bIsArray && m_vContents.count() == 0)
    {
        dumpIndentedNoNewLine(stream, iIdent, "[]");
    }
    else
    {
        if (isNamed())
        {
            dumpIndentedNoNewLine(stream, iIdent, "");
            m_pName->toQML(stream, pContext, this, iIdent);
        }

        if (m_bIsArray)
            dumpOpenArray(stream, iIdent - 1);
        else if (m_bIsObject || m_bIsBlock || isNamed())
            dumpOpenBlock(stream, iIdent - 1);

        if (m_bIsParenthesized)
        {
            dumpNoIndentNoNewLine(stream, "(");
        }

        int iCount = 0;

        foreach (QMLEntity* pItem, m_vContents)
        {
            if (pItem != nullptr)
            {
                if (m_bIsArray || m_bIsObject || m_bIsArgumentList)
                {
                    if (iCount > 0)
                    {
                        dumpNoIndentNoNewLine(stream, ", ");
                    }
                }

                if (m_bIsBlock)
                {
                    dumpIndentedNoNewLine(stream, iIdent, "");
                }

                pItem->toQML(stream, pContext, this, pParent != nullptr ? iIdent + 1 : iIdent);

                if (m_bIsBlock)
                {
                    dumpNewLine(stream);
                }

                iCount++;
            }
        }

        if (m_bIsParenthesized)
        {
            dumpNoIndentNoNewLine(stream, ")");
        }

        if (m_bIsArray)
            dumpCloseArray(stream, iIdent);
        else if (m_bIsObject || m_bIsBlock || isNamed())
            dumpCloseBlock(stream, iIdent);
    }
}

//-------------------------------------------------------------------------------------------------

CXMLNode QMLComplexEntity::toXMLNode(CXMLNodableContext* pContext, CXMLNodable* pParent)
{
    CXMLNode xNode = QMLEntity::toXMLNode(pContext, pParent);

    if (m_pName != nullptr)
    {
        xNode.attributes()["Name"] = m_pName->value().toString();
    }

    if (m_bIsArray)
        xNode.attributes()["IsArray"] = "true";

    if (m_bIsObject)
        xNode.attributes()["IsObject"] = "true";

    if (m_bIsBlock)
        xNode.attributes()["IsBlock"] = "true";

    if (m_bIsArgumentList)
        xNode.attributes()["IsArgumentList"] = "true";

    foreach (QMLEntity* pItem, m_vContents)
    {
        if (pItem != nullptr)
        {
            CXMLNode xChild = pItem->toXMLNode(pContext, this);
            xNode.nodes() << xChild;
        }
    }

    return xNode;
}

//-------------------------------------------------------------------------------------------------

QMLComplexEntity* QMLComplexEntity::fromItem(QMLEntity* pItem)
{
    QMLComplexEntity* pComplex = dynamic_cast<QMLComplexEntity*>(pItem);

    if (pComplex == nullptr)
    {
        pComplex = new QMLComplexEntity(pItem->position());
        pComplex->contents() << pItem;
    }

    return pComplex;
}

//-------------------------------------------------------------------------------------------------

QMLComplexEntity* QMLComplexEntity::makeBlock(QMLEntity* pItem)
{
    QMLComplexEntity* pComplex = fromItem(pItem);

    pComplex->setIsBlock(true);

    return pComplex;
}