
// Qt
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

// Application
#include "CLogger.h"

//-------------------------------------------------------------------------------------------------
// Taille maximum d'un fichier log

#define LOGGER_MAX_FILE_INDEX	5
#define LOGGER_MAX_FILE_SIZE	(10 * 1024 * 1024)	// 4 mb

//-------------------------------------------------------------------------------------------------

CLogger::CLogger()
    : m_tMutex(QMutex::Recursive)
    , m_tTimer(this)
    , m_tFlushTimer(this)
    , m_pFile(NULL)
    , m_iFileSize(0)
    , m_eLogLevel(llDebug)
    , m_bBackupActive(false)
{
    QString sName = QCoreApplication::applicationFilePath().split("/").last();
    QString sPath = QCoreApplication::applicationDirPath() + "/Logs";
    QString sFinal = sName + ".log";

    /*
    initialize(
                sPath,
                sFinal,
                CPreferencesManager::getInstance()->getPreferences().getNodeByTagName("Logger")
                );
                */

    initialize(sPath, sFinal, CXMLNode());

    connect(&m_tTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(&m_tFlushTimer, SIGNAL(timeout()), this, SLOT(onFlushTimeout()));

    m_tTimer.start(1000);
    m_tFlushTimer.start(2000);
}

//-------------------------------------------------------------------------------------------------

CLogger::CLogger(QString sPath, QString sFileName)
    : m_tMutex(QMutex::Recursive)
    , m_pFile(NULL)
    , m_iFileSize(0)
    , m_eLogLevel(llDebug)
{
    // initialize(sPath, sFileName, CPreferencesManager::getInstance()->getPreferences());
    initialize(sPath, sFileName, CXMLNode());

    connect(&m_tTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    m_tTimer.start(1000);
}

//-------------------------------------------------------------------------------------------------

void CLogger::registerChainedLogger(CLogger* pLogger)
{
    if (m_vChainedLoggers.contains(pLogger) == false)
    {
        m_vChainedLoggers.append(pLogger);
    }
}

//-------------------------------------------------------------------------------------------------

CLogger::~CLogger()
{
    // !!!! DON'T LOG IN THE DESTRUCTOR !!!!

    QMutexLocker locker(&m_tMutex);

    if (m_pFile)
    {
        if (m_pFile->isOpen())
        {
            m_pFile->close();
        }

        delete m_pFile;
    }
}

//-------------------------------------------------------------------------------------------------

QString CLogger::getPathName() const
{
    return m_sPathName;
}

//-------------------------------------------------------------------------------------------------

QString CLogger::getFileName() const
{
    return m_sFileName;
}

//-------------------------------------------------------------------------------------------------

void CLogger::initialize(QString sPath, QString sFileName, CXMLNode xParameters)
{
#ifndef NO_LOGGING

    QMutexLocker locker(&m_tMutex);

    CXMLNode xTokensNode = xParameters.getNodeByTagName(LOGGER_PARAM_TOKENS);
    CXMLNode xBackupNode = xParameters.getNodeByTagName(LOGGER_PARAM_BACKUP);

    // Read parameters
    if (xParameters.attributes()[LOGGER_PARAM_LEVEL].isEmpty() == false)
    {
        setLevel(xParameters.attributes()[LOGGER_PARAM_LEVEL]);
    }

    if (xTokensNode.attributes()[LOGGER_PARAM_DISPLAY].isEmpty() == false)
    {
        setDisplayTokens(xTokensNode.attributes()[LOGGER_PARAM_DISPLAY]);
    }

    if (xTokensNode.attributes()[LOGGER_PARAM_IGNORE].isEmpty() == false)
    {
        setIgnoreTokens(xTokensNode.attributes()[LOGGER_PARAM_IGNORE]);
    }

    if (xBackupNode.attributes()[LOGGER_PARAM_ACTIVE].isEmpty() == false)
    {
        m_bBackupActive = (bool) xBackupNode.attributes()[LOGGER_PARAM_ACTIVE].toInt();
    }

    // Assign file name
    m_sPathName = sPath;
    m_sFileName = sPath + "/" + sFileName;

    // Create target folder if needed
    if (QDir().exists(sPath) == false)
    {
        QDir().mkpath(sPath);
    }

    // Backups
    manageBackups(sFileName);

    // Destroy the file
    if (m_pFile)
    {
        if (m_pFile->isOpen())
        {
            m_pFile->close();
        }
        delete m_pFile;
    }

    // Create the file
    m_pFile = new QFile(m_sFileName);
    m_pFile->open(QIODevice::WriteOnly | QIODevice::Text);
    m_iFileSize = 0;

#endif
}

//-------------------------------------------------------------------------------------------------

void CLogger::manageBackups(const QString& sFileName)
{
    /*
    if (m_bBackupActive)
    {
        QDateTime tTime = QDateTime::currentDateTime();

        QString sCopyFileName = QString("%1.%2-%3-%4_%5-%6-%7.bak")
                .arg(sFileName)
                .arg(tTime.date().year())
                .arg(tTime.date().month())
                .arg(tTime.date().day())
                .arg(tTime.time().hour())
                .arg(tTime.time().minute())
                .arg(tTime.time().second());

        QFile tFile(sFileName);

        if (tFile.exists()) tFile.copy(sCopyFileName);
    }
    */
}

//-------------------------------------------------------------------------------------------------

void CLogger::setLevel(ELogLevel eLevel)
{
    QMutexLocker locker(&m_tMutex);

    m_eLogLevel = eLevel;
}

//-------------------------------------------------------------------------------------------------

void CLogger::setLevel(const QString& sLevel)
{
    QMutexLocker locker(&m_tMutex);

    if (sLevel.toLower() == "debug")
    {
        setLevel(llDebug);
    }
    else if (sLevel.toLower() == "info")
    {
        setLevel(llInfo);
    }
    else if (sLevel.toLower() == "warning")
    {
        setLevel(llWarning);
    }
    else if (sLevel.toLower() == "error")
    {
        setLevel(llError);
    }
    else if (sLevel.toLower() == "critical")
    {
        setLevel(llCritical);
    }
    else if (sLevel.toLower() == "always")
    {
        setLevel(llAlways);
    }
}

//-------------------------------------------------------------------------------------------------

void CLogger::setDisplayTokens(const QString& sTokens)
{
    if (sTokens == "")
    {
        m_sDisplayTokens.clear();
    }
    else
    {
        m_sDisplayTokens = sTokens.split("|");
    }
}

//-------------------------------------------------------------------------------------------------

void CLogger::setIgnoreTokens(const QString& sTokens)
{
    if (sTokens == "")
    {
        m_sIgnoreTokens.clear();
    }
    else
    {
        m_sIgnoreTokens = sTokens.split("|");
    }
}

//-------------------------------------------------------------------------------------------------

QString CLogger::getShortStringForLevel(ELogLevel eLevel, const QString& sText)
{
    QString sLogLevel;

    switch (eLevel)
    {
    case llDebug	: sLogLevel = "DEBUG"; break;
    case llInfo		: sLogLevel = "INFO"; break;
    case llWarning	: sLogLevel = "WARNING"; break;
    case llError	: sLogLevel = "ERROR"; break;
    case llCritical	: sLogLevel = "CRITICAL"; break;
    case llAlways	: sLogLevel = "ALWAYS"; break;
    }

    QString sFinalText = QString("[%1] - %2\n")
            .arg(sLogLevel)
            .arg(sText);

    return sFinalText;
}

//-------------------------------------------------------------------------------------------------

QString CLogger::getFinalStringForLevel(ELogLevel eLevel, const QString& sText)
{
    QString sLogLevel;

    switch (eLevel)
    {
    case llDebug	: sLogLevel = "DEBUG"; break;
    case llInfo		: sLogLevel = "INFO"; break;
    case llWarning	: sLogLevel = "WARNING"; break;
    case llError	: sLogLevel = "ERROR"; break;
    case llCritical	: sLogLevel = "CRITICAL"; break;
    case llAlways	: sLogLevel = "ALWAYS"; break;
    }

    QDateTime tNow = QDateTime::currentDateTime();

    QString sFinalText = QString("%1-%2-%3 %4:%5:%6.%7 [%8] - %9\n")
            .arg(tNow.date().year())
            .arg(tNow.date().month())
            .arg(tNow.date().day())
            .arg(tNow.time().hour())
            .arg(tNow.time().minute())
            .arg(tNow.time().second())
            .arg(tNow.time().msec())
            .arg(sLogLevel)
            .arg(sText);

    return sFinalText;
}

//-------------------------------------------------------------------------------------------------

bool CLogger::filterToken(QString sToken)
{
    if (m_sDisplayTokens.size() > 0)
    {
        bool bFound = false;

        foreach (QString sDisplay, m_sDisplayTokens)
        {
            if (sToken.contains(sDisplay, Qt::CaseInsensitive))
            {
                bFound = true;
                break;
            }
        }

        return bFound;
    }

    if (m_sIgnoreTokens.size() > 0)
    {
        bool bFound = false;

        foreach (QString sDisplay, m_sIgnoreTokens)
        {
            if (sToken.contains(sDisplay, Qt::CaseInsensitive))
            {
                bFound = true;
                break;
            }
        }

        return !bFound;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------

void CLogger::log(ELogLevel eLevel, const QString& sText, const QString& sToken)
{
    QMutexLocker locker(&m_tMutex);
    QString sFinalText = sText;

    // Est-ce que le token passe?
    if (filterToken(sToken) == false) return;

    // Ajout du token dans le texte
    if (sToken != "")
    {
        sFinalText = "<" + sToken + "> " + sText;
    }

    // Remplacement de noms ind?sirables
    sFinalText = sFinalText.replace("\"", "'");

    if (eLevel >= llError)
    {
        qDebug() << getShortStringForLevel(eLevel, sFinalText).toLatin1().constData();
    }

    if (m_pFile && m_pFile->isOpen())
    {
        switch (eLevel)
        {
        case llDebug :
        {
            if (m_eLogLevel <= llDebug)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llDebug, sFinalText).toLatin1().constData());
            }
        }
            break;

        case llInfo :
        {
            if (m_eLogLevel <= llInfo)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llInfo, sFinalText).toLatin1().constData());
            }
        }
            break;

        case llWarning :
        {
            if (m_eLogLevel <= llWarning)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llWarning, sFinalText).toLatin1().constData());

                foreach (CLogger* pLogger, m_vChainedLoggers) { pLogger->log(eLevel, sFinalText, sToken); }
            }
        }
            break;

        case llError :
        {
            if (m_eLogLevel <= llError)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llError, sFinalText).toLatin1().constData());

                foreach (CLogger* pLogger, m_vChainedLoggers) { pLogger->log(eLevel, sFinalText, sToken); }
            }
        }
            break;

        case llCritical :
        {
            if (m_eLogLevel <= llCritical)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llCritical, sFinalText).toLatin1().constData());

                foreach (CLogger* pLogger, m_vChainedLoggers) { pLogger->log(eLevel, sFinalText, sToken); }
            }
        }
            break;

        case llAlways :
        {
            if (m_eLogLevel <= llAlways)
            {
                m_iFileSize += m_pFile->write(getFinalStringForLevel(llAlways, sFinalText).toLatin1().constData());

                foreach (CLogger* pLogger, m_vChainedLoggers) { pLogger->log(eLevel, sFinalText, sToken); }
            }
        }
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void CLogger::flush()
{
    QMutexLocker locker(&m_tMutex);

    if (m_pFile != NULL)
    {
        m_pFile->flush();
    }
}

//-------------------------------------------------------------------------------------------------

void CLogger::logBuffer(ELogLevel eLevel, const char* pBuffer, int iSize)
{
    QMutexLocker locker(&m_tMutex);

    const char* pPointer = pBuffer;

    log(eLevel, "Buffer :");

    while (pPointer < pBuffer + iSize)
    {
        QString sText = "";

        for (int iIndex = 0; iIndex < 16 && (pPointer < pBuffer + iSize); iIndex++)
        {
            if (sText != "") sText += " ";
            sText += QString("0x%1").arg(*((const unsigned char*) pPointer), 2 , 16 , QChar('0'));
            pPointer++;
        }

        log(eLevel, sText);
    }
}

//-------------------------------------------------------------------------------------------------

void CLogger::onTimeout()
{
    QMutexLocker locker(&m_tMutex);

    if (m_pFile != NULL)
    {
        if (m_iFileSize > LOGGER_MAX_FILE_SIZE)
        {
            // Destruction du fichier
            m_pFile->close();
            delete m_pFile;

            // handleRollingCopies();

            // Cr?ation du fichier
            m_pFile = new QFile(m_sFileName);
            m_pFile->open(QIODevice::WriteOnly | QIODevice::Text);
            m_iFileSize = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CLogger::onFlushTimeout()
{
    flush();
}
