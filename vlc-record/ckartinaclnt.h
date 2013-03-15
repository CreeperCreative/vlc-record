/*=============================================================================\
| $HeadURL$
|
| Author: Jo2003
|
| last changed by: $Author$
|
| Begin: Monday, January 04, 2010 16:11:14
|
| $Id$
|
\=============================================================================*/
#ifndef __201004161114_CKARTINACLNT_H
   #define __201004161114_CKARTINACLNT_H

#include <QString>
#include <QDate>
#include <QRegExp>
#include <QMetaEnum>

#include "qiptvctrlclient.h"
#include "clogfile.h"
#include "defdef.h"

//---------------------------------------------------------------------------
//! \class   Kartina
//! \date    04.06.2012
//! \author  Jo2003
//! \brief   pseudo class for Qt's Meta Data Handling
//!          (replaces Kartina namespace)
//---------------------------------------------------------------------------
class Kartina : public QObject
{
   Q_OBJECT
   Q_ENUMS(EReq)
   Q_ENUMS(EErr)

public:
   //---------------------------------------------------------------------------
   //! request types used in client
   //---------------------------------------------------------------------------
   enum EReq
   {
      REQ_COOKIE,
      REQ_CHANNELLIST,
      REQ_STREAM,
      REQ_TIMESHIFT,
      REQ_EPG,
      REQ_SERVER,
      REQ_HTTPBUFF,
      REQ_ARCHIV,
      REQ_TIMERREC,
      REQ_GET_SERVER,
      REQ_LOGOUT,
      REQ_GETTIMESHIFT,
      REQ_GETVODGENRES,
      REQ_GETVIDEOS,
      REQ_GETVIDEOINFO,
      REQ_GETVODURL,
      REQ_GETBITRATE,
      REQ_SETBITRATE,
      REQ_CHANLIST_ALL,
      REQ_SETCHAN_HIDE,
      REQ_SETCHAN_SHOW,
      REQ_GET_VOD_MANAGER,
      REQ_SET_VOD_MANAGER,
      REQ_ADD_VOD_FAV,
      REQ_REM_VOD_FAV,
      REQ_GET_VOD_FAV,
      REQ_SET_PCODE,
      REQ_EPG_CURRENT,
      REQ_UNKNOWN = 255
   };

   //---------------------------------------------------------------------------
   //! Errors generated by Kartina.TV API
   //---------------------------------------------------------------------------
   enum EErr
   {
      ERR_HTTP = -1,
      ERR_UNKNOWN,
      ERR_INCORRECT_REQUEST,
      ERR_WRONG_LOGIN_DATA,
      ERR_ACCESS_DENIED,
      ERR_LOGIN_INCORRECT,
      ERR_CONTRACT_INACTIVE,
      ERR_CONTRACT_PAUSED,
      ERR_CHANNEL_NOT_FOUND,
      ERR_BAD_PARAM,
      ERR_MISSING_PARAM_DAY,
      ERR_MISSING_PARAM_CID,
      ERR_MULTIPLE_ACCOUNT_USE,
      ERR_AUTHENTICATION,
      ERR_PACKAGE_EXPIRED,
      ERR_UNKNOWN_API_FUNCTION,
      ERR_ARCHIVE_NOT_AVAIL,
      ERR_MISSING_PARAM_PLACE,
      ERR_MISSING_PARAM_NAME,
      ERR_CONFIRMATION_CODE,
      ERR_WRONG_PCODE,
      ERR_NEW_CODE,
      ERR_MISSING_PARAM_VAL,
      ERR_VALUE_NOT_ALLOWED,
      ERR_MISSING_PARAM,
      ERR_MISSING_PARAM_ID,
      ERR_MISSING_PARAM_FILEID,
      ERR_MISSING_PARAM_TYPE,
      ERR_MISSING_PARAM_QUERY,
      ERR_REMOVED_1,
      ERR_BITRATE_NOT_AVAIL,
      ERR_SERVICE_NOT_AVAIL,
      ERR_QUERY_LIMIT_EXCEEDED,
      ERR_RULE_ALREADY_EXISTS,
      ERR_RULE_NEED_CMD,
      ERR_MANAGE_NEED_CMD,
      ERR_MANAGE_BAD_VALUE,
      ERR_MANAGE_FILM_NOT_FOUND,
      ERR_MANAGE_ALREADY_ADDED
   };

   //---------------------------------------------------------------------------
   //! kartina request
   //---------------------------------------------------------------------------
   struct SRequest
   {
      EReq          req;
      QString       sOptArg1;
      QString       sOptArg2;
      int           iOptArg1;
      int           iOptArg2;
   };

   //---------------------------------------------------------------------------
   //
   //! \brief   get ascii text for enum value
   //
   //! \author  Jo2003
   //! \date    05.06.2012
   //
   //! \param   e EReq value
   //
   //! \return  ascii string for enum value
   //---------------------------------------------------------------------------
   const char* reqValToKey(EReq e)
   {
      QMetaEnum eReqEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator("EReq"));
      return eReqEnum.valueToKey(e);
   }

   //---------------------------------------------------------------------------
   //
   //! \brief   get ascii text for enum value
   //
   //! \author  Jo2003
   //! \date    05.06.2012
   //
   //! \param   e EErr value
   //
   //! \return  ascii string for enum value
   //---------------------------------------------------------------------------
   const char* errValToKey(EErr e)
   {
      QMetaEnum eErrEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator("EErr"));
      return eErrEnum.valueToKey(e);
   }
};

typedef QMap<Kartina::EErr, QString> QErrorMap;

/*=============================================================================\
| Class:       CKartinaClnt
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:12:28
|
| Description: a class to communicate with kartina.tv services
|
\=============================================================================*/
class CKartinaClnt : public QIptvCtrlClient
{
   Q_OBJECT

public:
   explicit CKartinaClnt(QObject *parent = 0);
   ~CKartinaClnt();

   void SetData(const QString &host, const QString &usr, const QString &pw);

   void queueRequest (Kartina::EReq req, int iArg1 = 0, int iArg2 = 0);
   void queueRequest (Kartina::EReq req, const QString &sReq1, const QString &sReq2 = QString());
   void queueRequest (Kartina::EReq req, int iArg1, const QString &sArg1);

   void fillErrorMap();
   bool cookieSet();
   void SetCookie (const QString &cookie);


protected:
   void queueIn(const Kartina::SRequest &req);
   void GetCookie ();
   void Logout ();
   void GetChannelList (const QString &secCode = QString());
   void SetTimeShift (int iHours);
   void GetTimeShift ();
   void GetStreamURL (int iChanID, const QString &secCode, bool bTimerRec = false);
   void GetArchivURL (const QString &prepared, const QString &secCode);
   void GetVodUrl (int iVidId, const QString &secCode);
   void GetVodGenres ();
   void SetServer (const QString& sIp);
   void GetServer ();
   void SetBitRate (int iRate);
   void GetBitRate ();
   void SetHttpBuffer (int iTime);
   void GetEPG (int iChanID, int iOffset = 0);
   void GetVideos (const QString &sPrepared);
   void GetVideoInfo (int iVodID, const QString &secCode = QString());
   void setChanHide (const QString &cids, const QString &secCode);
   void setChanShow (const QString &cids, const QString &secCode);
   void getVodManager (const QString &secCode);
   void setVodManager (const QString &rules, const QString &secCode);
   void addVodFav (int iVidID, const QString &secCode);
   void remVodFav (int iVidID, const QString &secCode);
   void getVodFav ();
   void setParentCode (const QString& oldCode, const QString& newCode);
   int  checkResponse (const QString &sResp, QString& sCleanResp);

   void epgCurrent(const QString &cids);

private:
   QString   sUsr;
   QString   sPw;
   QString   sApiUrl;
   QString   sCookie;
   QErrorMap errMap;
   Kartina   karTrace;

private slots:
   void slotStringResponse (int reqId, QString strResp);
   void slotBinResponse (int reqId, QByteArray binResp);
   void slotErr (QString sErr, int iErr);

signals:
   void sigError (QString str, int req, int err);
   void sigHttpResponse(QString str, int req);
};

#endif /* __201004161114_CKARTINACLNT_H */
/*=============================================================================\
|                                    History:
| ---------------------------------------------------------------------------
| 04.Jan.2010 - communication API for kartina.tv (inspired by conros)
\=============================================================================*/

