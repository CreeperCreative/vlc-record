/*------------------------------ Information ---------------------------*//**
 *
 *  $HeadURL$
 *
 *  @file     csundukclnt.cpp
 *
 *  @author   Jo2003
 *
 *  @date     12.12.2013
 *
 *  $Id$
 *
 *///------------------------- (c) 2013 by Jo2003  --------------------------
#include "csundukclnt.h"
#include "qcustparser.h"


// global customization class ...
extern QCustParser *pCustomization;

// log file functions ...
extern CLogFile VlcLog;

//---------------------------------------------------------------------------
//
//! \brief   constructs CSundukClnt object
//
//! \author  Jo2003
//! \date    12.12.2013
//
//! \param   parent (QObject*) pointer to parent object
//
//! \return  --
//---------------------------------------------------------------------------
CSundukClnt::CSundukClnt(QObject *parent) : CKartinaClnt(parent)
{
   sStrProto = "hls";
}

//---------------------------------------------------------------------------
//
//! \brief   destroys CSundukClnt object
//
//! \author  Jo2003
//! \date    12.12.2013
//
//! \param   --
//
//! \return  --
//---------------------------------------------------------------------------
CSundukClnt::~CSundukClnt()
{
   // nothing to do so far ...
}

//---------------------------------------------------------------------------
//
//! \brief   login
//
//! \author  Jo2003
//! \date    22.11.2014
//
//! \param   --
//
//! \return  --
//---------------------------------------------------------------------------
void CSundukClnt::GetCookie()
{
   mInfo(tr("Request Authentication ..."));

   q_post((int)CIptvDefs::REQ_COOKIE, apiUrl() + "login",
        QString("login=%1&pass=%2&settings=all&softid=%3%4-%5&deviceId=%6")
            .arg(sUsr).arg(sPw)
            .arg(pCustomization->strVal("APPLICATION_SHORTCUT"))
            .arg(OP_SYS).arg(SOFTID_DEVELOPER)
            .arg(getStbSerial()),
        Iptv::Login);
}

//---------------------------------------------------------------------------
//
//! \brief   get url for video stream
//
//! \author  Jo2003
//! \date    12.12.2013
//
//! \param   iChanID (int) channel id
//! \param   secCode (const QString &) protect code
//! \param   bTimerRec (bool) timer record flag
//
//! \return  --
//---------------------------------------------------------------------------
void CSundukClnt::GetStreamURL (int iChanID, const QString &secCode, bool bTimerRec)
{
   mInfo(tr("Request URL for channel %1 ...").arg(iChanID));

   QString req = QString("cid=%1&stream_protocol=%2").arg(iChanID).arg(sStrProto);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   q_post((bTimerRec) ? (int)CIptvDefs::REQ_TIMERREC : (int)CIptvDefs::REQ_STREAM,
          apiUrl() + "get_url", req);
}

//---------------------------------------------------------------------------
//
//! \brief   get url for archive stream
//
//! \author  Jo2003
//! \date    12.12.2013
//
//! \param   prepared (const QString &) prepared request
//! \param   secCode (const QString &) protect code
//
//! \return  --
//---------------------------------------------------------------------------
void CSundukClnt::GetArchivURL (const QString &prepared, const QString &secCode)
{
   mInfo(tr("Request Archiv URL ..."));

   QString req = QUrl::fromPercentEncoding(prepared.toUtf8());
   req += "&stream_protocol=" + sStrProto;

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   q_post((int)CIptvDefs::REQ_ARCHIV, apiUrl() + "get_url", req);
}

//---------------------------------------------------------------------------
//
//! \brief   get url for vod stream
//
//! \author  Jo2003
//! \date    12.12.2013
//
//! \param   iVidId (int) video id
//! \param   secCode (const QString &) protect code
//
//! \return  --
//---------------------------------------------------------------------------
void CSundukClnt::GetVodUrl (int iVidId, const QString &secCode)
{
   mInfo(tr("Request Video Url for video %1...").arg(iVidId));

   QString req = QString("vod_geturl?fileid=%1&ad=1&stream_protocol=%2")
         .arg(iVidId).arg(sStrProto);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   q_get((int)CIptvDefs::REQ_GETVODURL, apiUrl() + req);
}

//---------------------------------------------------------------------------
//
//! \brief   set stream protocol
//
//! \author  Jo2003
//! \date    22.11.2014
//
//! \param   p (QString) new protocol
//
//! \return  --
//---------------------------------------------------------------------------
void CSundukClnt::slotStrProto(QString p)
{
   sStrProto = p;
}
