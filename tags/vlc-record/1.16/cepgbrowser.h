/*********************** Information *************************\
| $HeadURL$
|
| Author: Jo2003
|
| Begin: 18.01.2010 / 16:05:56
|
| Last edited by: $Author$
|
| $Id$
\*************************************************************/
#ifndef __011810__CEPGBROWSER_H
   #define __011810__CEPGBROWSER_H

#include <QTextBrowser>
#include <QMap>
#include "ckartinaxmlparser.h"
#include "templates.h"

#include "clogfile.h"
#include "defdef.h"

/********************************************************************\
|  Class: CEpgBrowser
|  Date:  18.01.2010 / 16:06:32
|  Author: Jo2003
|  Description: textbrowser with epg functionality
|
\********************************************************************/
class CEpgBrowser : public QTextBrowser
{
   Q_OBJECT

public:
    CEpgBrowser(QWidget *parent = 0);
    void DisplayEpg(QVector<cparser::SEpg> epglist,
                    const QString &sName, int iChanID,
                    uint uiGmt, bool bHasArchiv);

    void SetTimeShift (int iTs) { iTimeShift = iTs; }
    void SetLogoDir (const QString &sDir) { sLogoDir = sDir; }
    int  GetCid () { return iCid; }
    QString ShowName (uint uiTimeT);
    void EnlargeFont ();
    void ReduceFont ();

protected:
    bool NowRunning (const QDateTime &startThis, const QDateTime &startNext = QDateTime());
    bool ArchivAvailable (uint uiThisShow);

private:
    int                 iTimeShift;
    int                 iCid;
    QString             sLogoDir;
    QMap<uint, QString> mProgram;
};

#endif /* __011810__CEPGBROWSER_H */
/************************* History ***************************\
| $Log$
\*************************************************************/
