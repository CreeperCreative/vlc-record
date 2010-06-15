/*********************** Information *************************\
| $HeadURL$
|
| Author: Jo2003
|
| Begin: 13.06.2010 / 14:50:35
|
| Last edited by: $Author$
|
| $Id$
\*************************************************************/
#ifndef __120610_VLC_REC_DB_H
   #define __120610_VLC_REC_DB_H

#include <QObject>
#include <QSqlDataBase>
#include <QMessageBox>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QVariant>

#include "cdirstuff.h"

#define VLC_REC_DB "vlcrec.db.sqlite"

/********************************************************************\
|  Class: CVlcRecDB
|  Date:  13.06.2010 / 16:00:28
|  Author: Jo2003
|  Description: class for database storage, inherits QProcess
|
\********************************************************************/
class CVlcRecDB : public QObject
{
   Q_OBJECT

public:
   CVlcRecDB();
   ~CVlcRecDB();
   int aspect (int iCid, int& iAspect, int &iCrop);
   int addAspect (int iCid, int iAspect, int iCrop);
   QString sqlError();

   /* -----------------------------------------------------------------\
   |  Method: setValue
   |  Begin: 13.06.2010 / 16:17:51
   |  Author: Jo2003
   |  Description: store setting in db
   |
   |  Parameters: ref. to key, ref. to value
   |
   |  Returns: 0 --> ok
   |          -1 --> error
   \----------------------------------------------------------------- */
   template <typename T> int setValue (const QString &sKey, const T& val)
   {
      QSqlQuery query;
      query.prepare("INSERT OR REPLACE INTO settings (name, val) VALUES (?, ?)");
      query.addBindValue(sKey);
      query.addBindValue(val);

      return query.exec() ? 0 : -1;
   }

   QString stringValue(const QString &sKey);
   int     intValue(const QString &sKey);
   float   floatValue(const QString &sKey);
   int     ask(const QString &question, QSqlQuery &query);

protected:
   int checkDb();
   int deleteDb();

private:
   QSqlDatabase db;
   QSqlQuery    qExp;
};

#endif // __120610_VLC_REC_DB
/************************* History ***************************\
| $Log$
\*************************************************************/

