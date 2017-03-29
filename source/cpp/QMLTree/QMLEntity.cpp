
// Qt
#include <QDebug>

// Application
#include "QMLEntity.h"

//-------------------------------------------------------------------------------------------------

/*!
    \class QMLEntity
    \inmodule unis-lib
    \brief The base item for a QML tree.
*/

//-------------------------------------------------------------------------------------------------

QList<QMLEntity*> QMLEntity::s_vEntities;
int QMLEntity::s_iCreatedEntities = 0;
int QMLEntity::s_iDeletedEntities = 0;

//-------------------------------------------------------------------------------------------------

/*!
    Constructs a QMLItem. \br\br
    \a pPosition is the position of the token in the file
*/
QMLEntity::QMLEntity(const QPoint& pPosition)
    : m_pPosition(pPosition)
    , m_pOrigin(nullptr)
    , m_iUsageCount(0)
    , m_bIsParenthesized(false)
{
#ifdef TRACK_ENTITIES
    s_iCreatedEntities++;
    s_vEntities << this;
#endif
}

//-------------------------------------------------------------------------------------------------

/*!
    Constructs a QMLItem with \a value.
*/
QMLEntity::QMLEntity(const QPoint& pPosition, const QVariant& value)
    : m_vValue(value)
    , m_pPosition(pPosition)
    , m_pOrigin(nullptr)
    , m_iUsageCount(0)
    , m_bIsParenthesized(false)
{
#ifdef TRACK_ENTITIES
    s_iCreatedEntities++;
    s_vEntities << this;
#endif
}

//-------------------------------------------------------------------------------------------------

/*!
    Destroys a QMLItem.
*/
QMLEntity::~QMLEntity()
{
#ifdef TRACK_ENTITIES
    s_iDeletedEntities++;
    s_vEntities.removeAll(this);
#endif
}

//-------------------------------------------------------------------------------------------------

/*!
    Sets the item's value to \a value.
*/
void QMLEntity::setValue(const QVariant& value)
{
    m_vValue = value;
}

//-------------------------------------------------------------------------------------------------

/*!
    Sets the item's position to \a point.
*/
void QMLEntity::setPosition(const QPoint& point)
{
    m_pPosition = point;
}

//-------------------------------------------------------------------------------------------------

/*!
    Sets the item's origin to \a pEntity.
*/
void QMLEntity::setOrigin(QMLEntity* pEntity)
{
    m_pOrigin = pEntity;
}

//-------------------------------------------------------------------------------------------------

/*!
    Sets the item's isParenthesized flag to \a bValue.
*/
void QMLEntity::setIsParenthesized(bool bValue)
{
    m_bIsParenthesized = bValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's position in the file.
*/
QPoint QMLEntity::position() const
{
    return m_pPosition;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's origin.
*/
QMLEntity* QMLEntity::origin() const
{
    return m_pOrigin;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's usage count.
*/
int QMLEntity::usageCount() const
{
    return m_iUsageCount;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's value.
*/
QVariant QMLEntity::value() const
{
    return m_vValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item's isParenthesized flag.
*/
bool QMLEntity::isParenthesized() const
{
    return m_bIsParenthesized;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item as a string.
*/
QString QMLEntity::toString() const
{
    return m_vValue.toString();
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a map of class members.
*/
QMap<QString, QMLEntity*> QMLEntity::members()
{
    return QMap<QString, QMLEntity*>();
}

//-------------------------------------------------------------------------------------------------

/*!
    Increments the usage count for this object.
*/
void QMLEntity::incUsageCount()
{
    m_iUsageCount++;
}

//-------------------------------------------------------------------------------------------------

QMLEntity* QMLEntity::clone() const
{
    QMLEntity* pEntity = new QMLEntity(m_pPosition);

    pEntity->m_vValue               = m_vValue;
    pEntity->m_pPosition            = m_pPosition;
    pEntity->m_pOrigin              = m_pOrigin;
    pEntity->m_iUsageCount          = m_iUsageCount;
    pEntity->m_bIsParenthesized     = m_bIsParenthesized;
}

//-------------------------------------------------------------------------------------------------

/*!
    Finds all symbols in the entity. \br\br
    \a pContext is the context of this entity. \br
*/
void QMLEntity::solveSymbols(QMLTreeContext* pContext)
{
    QMap<QString, QMLEntity*> mMembers = members();

    foreach (QString sMemberKey, mMembers.keys())
    {
        if (mMembers[sMemberKey] != nullptr)
        {
            mMembers[sMemberKey]->setParent(this);
            mMembers[sMemberKey]->solveSymbols(pContext);
        }
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Finds the origin of the entity. \br\br
    \a pContext is the context of this entity. \br
*/
void QMLEntity::solveReferences(QMLTreeContext* pContext)
{
    QMap<QString, QMLEntity*> mMembers = members();

    foreach (QString sMemberKey, mMembers.keys())
    {
        if (mMembers[sMemberKey] != nullptr)
        {
            mMembers[sMemberKey]->solveReferences(pContext);
        }
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Checks how many times each symbol is used. \br\br
    \a pContext is the context of this item.
*/
void QMLEntity::solveSymbolUsages(QMLTreeContext* pContext)
{
    QMap<QString, QMLEntity*> mMembers = members();

    foreach (QString sMemberKey, mMembers.keys())
    {
        if (mMembers[sMemberKey] != nullptr)
        {
            mMembers[sMemberKey]->solveSymbolUsages(pContext);
        }
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a list of all declared symbols.
*/
QMap<QString, QMLEntity*> QMLEntity::getDeclaredSymbols()
{
    QMap<QString, QMLEntity*> mReturnValue;

    QMap<QString, QMLEntity*> mMembers = members();

    foreach (QString sMemberKey, mMembers.keys())
    {
        if (mMembers[sMemberKey] != nullptr)
        {
            QMap<QString, QMLEntity*> memberSymbols = mMembers[sMemberKey]->getDeclaredSymbols();

            foreach (QString sSymbolKey, memberSymbols.keys())
            {
                if (mReturnValue.contains(sSymbolKey) == false)
                {
                    mReturnValue[sSymbolKey] = memberSymbols[sSymbolKey];
                }
            }
        }
    }

    return mReturnValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item named \a sName, for identifier resolution. \br\br
*/
QMLEntity* QMLEntity::findSymbolDeclaration(const QString& sName)
{
    QMLEntity* pParent = dynamic_cast<QMLEntity*>(parent());

    if (pParent != nullptr)
    {
        return pParent->findSymbolDeclaration(sName);
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item named \a lQualifiedName, for identifier resolution. \br\br
*/
QMLEntity* QMLEntity::findSymbolDeclarationDescending(QStringList& lQualifiedName)
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------

/*!
    Remove all unreferenced declarations in this entity. \br\br
    \a pContext is the context of this entity. \br
*/
void QMLEntity::removeUnreferencedSymbols(QMLTreeContext* pContext)
{
    QMap<QString, QMLEntity*> mMembers = members();

    foreach (QString sMemberKey, mMembers.keys())
    {
        if (mMembers[sMemberKey] != nullptr)
        {
            mMembers[sMemberKey]->removeUnreferencedSymbols(pContext);
        }
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Dumps the entity as QML to \a stream using \a iIdent for indentation. \br\br
    \a pContext is the context of this item. \br
    \a pParent is the caller of this method.
*/
void QMLEntity::toQML(QTextStream& stream, QMLTreeContext* pContext, QMLEntity* pParent, int iIdent)
{
    Q_UNUSED(pContext);
    Q_UNUSED(pParent);

    QString sValue;

    switch (m_vValue.type())
    {
        case QVariant::Int :
            sValue = m_vValue.toString();
            break;

        case QVariant::Double :
            sValue = m_vValue.toString();
            break;

        case QVariant::Bool :
            sValue = m_vValue.toBool() ? "true" : "false";
            break;

        case QVariant::String :
        case QVariant::Color :
        case QVariant::Point :
        case QVariant::PointF :
        case QVariant::Font :
            sValue = QString("\"%1\"").arg(m_vValue.toString());
    }

    if (sValue.isEmpty() == false)
    {
        stream << sValue;
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a CXMLNode representation of this item. \br\br
    \a pContext is a user defined context. \br
    \a pParent is the caller of this method.
*/
CXMLNode QMLEntity::toXMLNode(CXMLNodableContext* pContext, CXMLNodable* pParent)
{
    CXMLNode xNode(metaObject()->className());
    QString sValue = m_vValue.value<QString>();

    if (sValue.isEmpty() == false)
    {
        xNode.attributes()["Value"] = sValue;
    }

    if (m_bIsParenthesized)
    {
        xNode.attributes()["IsParenthesized"] = "true";
    }

    if (parent() == nullptr)
    {
        xNode.attributes()["Parent"] = "NULL";
    }

    if (m_pOrigin != nullptr)
    {
        xNode.attributes()["Origin"] = m_pOrigin->toString() + " (" + m_pOrigin->metaObject()->className() + ")";
    }

    if (m_iUsageCount > 0)
    {
        xNode.attributes()["UsageCount"] = QString::number(m_iUsageCount);
    }

    return xNode;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a string list with each dot separated component of \a sName.
*/
QStringList QMLEntity::qualifiedNameAsList(const QString& sName)
{
    return sName.split(".");
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns a string composed of each string in \a sNameList, separated with a dot.
*/
QString QMLEntity::listAsQualifiedName(const QStringList& sNameList)
{
    QString sReturnValue;

    foreach (QString sName, sNameList)
    {
        if (sReturnValue.isEmpty() == false)
        {
            sReturnValue += ".";
        }

        sReturnValue += sName;
    }

    return sReturnValue;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns number of created QMLEntity objects.
*/
int QMLEntity::entityCount()
{
    return s_vEntities.count();
}

/*!
    Returns number of created QMLEntity objects.
*/
int QMLEntity::createdEntities()
{
    return s_iCreatedEntities;
}

/*!
    Returns number of deleted QMLEntity objects.
*/
int QMLEntity::deletedEntities()
{
    return s_iDeletedEntities;
}

/*!
    Returns created QMLEntity objects.
*/
QList<QMLEntity*>& QMLEntity::entities()
{
    return s_vEntities;
}
