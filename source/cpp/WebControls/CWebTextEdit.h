
#ifndef CWEBTEXTEDIT_H
#define CWEBTEXTEDIT_H

// Application
#include "CWebControl.h"

class QTPLUSSHARED_EXPORT CWebTextEdit : public CWebControl
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructeurs et destructeur
    //-------------------------------------------------------------------------------------------------

    //!
    static CWebControl* instantiator();

    //! Constructeur par d�faut
    CWebTextEdit();

    //! Constructeur avec param�tres
    CWebTextEdit(const QString& sName, const QString& sCaption);

    //! Destructeur
    virtual ~CWebTextEdit();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // M�thodes de contr�le
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // M�thodes h�rit�es
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void addHTML(QString& sHead, QString& sBody) const;

    //!
    virtual void handleEvent(QString sControl, QString sEvent, QString sParam);

    //-------------------------------------------------------------------------------------------------
    // Propri�t�s
    //-------------------------------------------------------------------------------------------------

protected:
};

#endif // CWEBTEXTEDIT_H
