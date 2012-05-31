/*=============================================================================\
| $HeadURL$
|
| Author: Jo2003
|
| last changed by: $Author$
|
| Begin: Monday, January 04, 2010 16:13:58
|
| $Id$
|
\=============================================================================*/
#include "ckartinaclnt.h"

// log file functions ...
extern CLogFile VlcLog;

/*-----------------------------------------------------------------------------\
| Function:    CKartinaClnt / constructor
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: constructs a CKartinaClnt object to communicate with
|              kartina.tv
| Parameters:  host, username, password, erotic allowed (true / false)
|
\-----------------------------------------------------------------------------*/
CKartinaClnt::CKartinaClnt(const QString &host, const QString &usr,
                           const QString &pw, const QString &sEPw,
                           bool bAllowErotic) :QHttp(host)
{
   sUsr           = usr;
   sPw            = pw;
   sErosPw        = sEPw;
   iReq           = -1;
   bEros          = bAllowErotic;
   sCookie        = "";
   sHost          = host;
   eReq           = Kartina::REQ_UNKNOWN;

   bufReq.open(QIODevice::WriteOnly);

   connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(handleEndRequest(int, bool)));
}

/*-----------------------------------------------------------------------------\
| Function:    CKartinaClnt / constructor
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: constructs a CKartinaClnt object to communicate with
|              kartina.tv
| Parameters:  --
|
\-----------------------------------------------------------------------------*/
CKartinaClnt::CKartinaClnt() :QHttp()
{
   sUsr           = "";
   sPw            = "";
   sErosPw        = "";
   iReq           = -1;
   bEros          = false;
   sCookie        = "";
   sHost          = "";
   eReq           = Kartina::REQ_UNKNOWN;

   bufReq.open(QIODevice::WriteOnly);

   connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(handleEndRequest(int, bool)));
}

/*-----------------------------------------------------------------------------\
| Function:    ~CKartinaClnt / destructor
|
| Author:      Jo2003
|
| Begin:       Thursday, January 07, 2010 11:54:52
|
| Description: clean at destruction ...
|
| Parameters:  --
|
\-----------------------------------------------------------------------------*/
CKartinaClnt::~CKartinaClnt()
{
   abort();
}

/*-----------------------------------------------------------------------------\
| Function:    SetData
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: set communication parameter to communicate with
|              kartina.tv
| Parameters:  host, username, password, erotic allowed (true / false)
|
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetData(const QString &host, const QString &usr,
                           const QString &pw, const QString &sEPw,
                           bool bAllowErotic)
{
   sUsr           = usr;
   sPw            = pw;
   sErosPw        = sEPw;
   bEros          = bAllowErotic;
   sHost          = host;
   sCookie        = "";
   eReq           = Kartina::REQ_UNKNOWN;

   setHost(host);
}

/*-----------------------------------------------------------------------------\
| Function:    SetCookie
|
| Author:      Jo2003
|
| Begin:       03.03.2010 / 12:18:00
|
| Description: set a former stored cookie
|
| Parameters:  ref. cookie string
|
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetCookie(const QString &cookie)
{
   mInfo(tr("We've got following Cookie: %1").arg(cookie));
   sCookie      = cookie;
}

/*-----------------------------------------------------------------------------\
| Function:    Logout
|
| Author:      Jo2003
|
| Begin:       28.07.2010 / 12:18:00
|
| Description: logout from kartina
|
| Parameters:  --
|
\-----------------------------------------------------------------------------*/
void CKartinaClnt::Logout ()
{
   mInfo(tr("Logout ..."));

   GetRequest(Kartina::REQ_LOGOUT, KARTINA_API_PATH "logout");
}

/*-----------------------------------------------------------------------------\
| Function:    PostRequest
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: send a post request to kartina.tv
|
| Parameters:  request type, path on server, request string
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::PostRequest (Kartina::EReq req,
                                const QString &path,
                                const QString &content,
                                const QString &sBrowser)
{
   eReq = req;
   QHttpRequestHeader header("POST", path.toAscii());
   header.addValue("Host", sHost);
   header.setContentType("application/x-www-form-urlencoded");
   header.addValue("User-Agent", sBrowser);
   header.addValue("Connection", "close");
   if (sCookie != "")
   {
      header.addValue("Cookie", sCookie);
   }
   header.setContentLength(content.toAscii().size());

   // open and clean data buffer ...
   bufReq.open(QIODevice::WriteOnly | QIODevice::Truncate);

   // post request ...
   iReq = request(header, content.toAscii(), &bufReq);

   mInfo(tr("Request #%1 (%2) sent ...").arg(iReq).arg (eReq));
}

/*-----------------------------------------------------------------------------\
| Function:    GetRequest
|
| Author:      Jo2003
|
| Begin:       28.07.2010 / 16:14:52
|
| Description: send a get request to kartina.tv
|
| Parameters:  request type, request
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetRequest (Kartina::EReq req,
                               const QString &sRequest,
                               const QString &sBrowser)
{
   eReq = req;
   QHttpRequestHeader header("GET", sRequest.toAscii());
   header.addValue("Host", sHost);
   header.setContentType("application/x-www-form-urlencoded");
   header.addValue("User-Agent", sBrowser);
   header.addValue("Connection", "close");
   if (sCookie != "")
   {
      header.addValue("Cookie", sCookie);
   }
   header.setContentLength(0);

   // open and clean data buffer ...
   bufReq.open(QIODevice::WriteOnly | QIODevice::Truncate);

   // post request ...
   iReq = request(header, QByteArray(), &bufReq);

   mInfo(tr("Request #%1 (%2) sent ...").arg(iReq).arg (eReq));
}

/*-----------------------------------------------------------------------------\
| Function:    GetCookie
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: request authentication
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetCookie ()
{
   mInfo(tr("Request Authentication ..."));

   PostRequest(Kartina::REQ_COOKIE, KARTINA_API_PATH "login",
               QString("login=%1&pass=%2&settings=all").arg(sUsr).arg(sPw));
}

/*-----------------------------------------------------------------------------\
| Function:    GetChannelList
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: request channel list from kartina.tv
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetChannelList (const QString &secCode)
{
   mInfo(tr("Request Channel List ..."));
   QString req;

   if (secCode != "")
   {
      // normal channel list request ...
      req = QString("show=all&protect_code=%1").arg(secCode);
   }

   // request channel list or channel list for settings ...
   PostRequest((secCode == "") ? Kartina::REQ_CHANNELLIST : Kartina::REQ_CHANLIST_ALL,
               KARTINA_API_PATH "channel_list", req);
}

/*-----------------------------------------------------------------------------\
| Function:    GetServer
|
| Author:      Jo2003
|
| Begin:       17.02.2010, 18:30:52
|
| Description: request stream server list from kartina
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetServer()
{
   mInfo(tr("Request Stream Server List ..."));

   GetRequest(Kartina::REQ_GET_SERVER, KARTINA_API_PATH "settings?var=stream_server");
}

/*-----------------------------------------------------------------------------\
| Function:    GetTimeShift
|
| Author:      Jo2003
|
| Begin:       29.07.2010, 13:50:52
|
| Description: request timeshift from kartina server
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetTimeShift()
{
   mInfo(tr("Request Time Shift ..."));

   GetRequest(Kartina::REQ_GETTIMESHIFT, KARTINA_API_PATH "settings?var=timeshift");
}

/*-----------------------------------------------------------------------------\
| Function:    SetTimeShift
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: set timeshift
|
| Parameters:  int value in hours (allowed: 0, 1, 2, 3, 4, 8, 9, 10, 11)
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetTimeShift (int iHours)
{
   mInfo(tr("Set TimeShift to %1 hour(s) ...").arg(iHours));

   PostRequest(Kartina::REQ_TIMESHIFT, KARTINA_API_PATH "settings_set",
               QString("var=timeshift&val=%1").arg(iHours));
}

/*-----------------------------------------------------------------------------\
| Function:    GetBitRate
|
| Author:      Jo2003
|
| Begin:       14.01.2011, 13:50:52
|
| Description: request bitrate from kartina server
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetBitRate()
{
   mInfo(tr("Request Bit Rate ..."));

   GetRequest(Kartina::REQ_GETBITRATE, KARTINA_API_PATH "settings?var=bitrate");
}

/*-----------------------------------------------------------------------------\
| Function:    SetBitRate
|
| Author:      Jo2003
|
| Begin:       14.01.2011 / 14:05
|
| Description: set bit rate
|
| Parameters:  int value kbit/sec. (allowed: 900, 1500)
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetBitRate(int iRate)
{
   mInfo(tr("Set BitRate to %1 kbit/s ...").arg(iRate));

   PostRequest(Kartina::REQ_SETBITRATE, KARTINA_API_PATH "settings_set",
               QString("var=bitrate&val=%1").arg(iRate));
}

/*-----------------------------------------------------------------------------\
| Function:    GetStreamURL
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: request stream URL for given channel id
|
| Parameters:  channel index
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetStreamURL(int iChanID, bool bTimerRec)
{
   mInfo(tr("Request URL for channel %1 ...").arg(iChanID));

   QString req = QString("cid=%1").arg(iChanID);

   if (bEros)
   {
      req += QString("&protect_code=%1").arg(sErosPw);
   }

   PostRequest((bTimerRec) ? Kartina::REQ_TIMERREC : Kartina::REQ_STREAM,
               KARTINA_API_PATH "get_url", req);
}

/*-----------------------------------------------------------------------------\
| Function:    SetServer
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: set streaming server
|
| Parameters:  server number (1 or 3)
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetServer (const QString &sIp)
{
   mInfo(tr("Set Streaming Server to %1 ...").arg(sIp));

   PostRequest(Kartina::REQ_SERVER, KARTINA_API_PATH "settings_set",
               QString("var=stream_server&val=%1").arg(sIp));
}

/*-----------------------------------------------------------------------------\
| Function:    SetHttpBuffer
|
| Author:      Jo2003
|
| Begin:       Thursday, January 21, 2010 11:49:52
|
| Description: set http buffer time
|
| Parameters:  time in msec. (1500, 3000, 5000, 8000, 15000)
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::SetHttpBuffer(int iTime)
{
   mInfo(tr("Set Http Buffer to %1 msec. ...").arg(iTime));

   PostRequest(Kartina::REQ_HTTPBUFF, KARTINA_API_PATH "settings_set",
               QString("var=http_caching&val=%1").arg(iTime));
}

/*-----------------------------------------------------------------------------\
| Function:    GetEPG
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: request todays EPG for given channel id
|
| Parameters:  channel index
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetEPG(int iChanID, int iOffset)
{
   mInfo(tr("Request EPG for Channel %1 ...").arg(iChanID));

   QDate now = QDate::currentDate().addDays(iOffset);

   GetRequest(Kartina::REQ_EPG,
              QString("%1epg?cid=%2&day=%3").arg(KARTINA_API_PATH)
              .arg(iChanID).arg(now.toString("ddMMyy")));
}

/*-----------------------------------------------------------------------------\
| Function:    GetArchivURL
|
| Author:      Jo2003
|
| Begin:       Monday, January 18, 2010 14:57:52
|
| Description: request archiv URL for prepared request
|
| Parameters:  prepared request
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetArchivURL (const QString &prepared)
{
   mInfo(tr("Request Archiv URL ..."));

   QString req = prepared;

   if (bEros)
   {
      req += QString("&protect_code=%1").arg(sErosPw);
   }

   PostRequest(Kartina::REQ_ARCHIV, KARTINA_API_PATH "get_url", req);
}

/*-----------------------------------------------------------------------------\
| Function:    GetVodGenres
|
| Author:      Jo2003
|
| Begin:       09.12.2010 / 13:18
|
| Description: request VOD genres (for now answer will be html code)
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetVodGenres()
{
   mInfo(tr("Request VOD Genres ..."));

   GetRequest(Kartina::REQ_GETVODGENRES, KARTINA_API_PATH "vod_genres");
}

/*-----------------------------------------------------------------------------\
| Function:    GetVideos
|
| Author:      Jo2003
|
| Begin:       09.12.2010 / 13:18
|
| Description: get vidoes matching to prepared search string
|
| Parameters:  prepared search string
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetVideos(const QString &sPrepared)
{
   mInfo(tr("Request Videos ..."));

   QString sReq = QString("%1vod_list?%2")
         .arg(KARTINA_API_PATH).arg(sPrepared);

   GetRequest(Kartina::REQ_GETVIDEOS, sReq);
}

/*-----------------------------------------------------------------------------\
| Function:    GetVideoInfo
|
| Author:      Jo2003
|
| Begin:       21.12.2010 / 16:18
|
| Description: get video info for video id (VOD)
|
| Parameters:  video id
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetVideoInfo(int iVodID, const QString &secCode)
{
   mInfo(tr("Request Video info for video %1...").arg(iVodID));

   QString req = QString("%1vod_info?id=%2").arg(KARTINA_API_PATH).arg(iVodID);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   GetRequest(Kartina::REQ_GETVIDEOINFO, req);
}

/*-----------------------------------------------------------------------------\
| Function:    GetVodUrl
|
| Author:      Jo2003
|
| Begin:       22.12.2010 / 16:18
|
| Description: get video url for video id (VOD)
|
| Parameters:  video id
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::GetVodUrl(int iVidId, const QString &secCode)
{
   mInfo(tr("Request Video Url for video %1...").arg(iVidId));

   QString req = QString("%1vod_geturl?fileid=%2&ad=1")
         .arg(KARTINA_API_PATH)
         .arg(iVidId);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   GetRequest(Kartina::REQ_GETVODURL, req);
}

/*-----------------------------------------------------------------------------\
| Function:    setChanHide
|
| Author:      Jo2003
|
| Begin:       14.05.20012
|
| Description: hide channel from channel list
|
| Parameters:  channel id(s), security code
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::setChanHide(const QString &cids, const QString &secCode)
{
   mInfo(tr("Hide channel(s) %1 from channel list ...").arg(cids));

   QString req = QString("cmd=hide_channel&cid=%1&protect_code=%2")
           .arg(cids).arg(secCode);

   PostRequest(Kartina::REQ_SETCHAN_HIDE, KARTINA_API_PATH "rule", req);
}

/*-----------------------------------------------------------------------------\
| Function:    setChanShow
|
| Author:      Jo2003
|
| Begin:       14.05.20012
|
| Description: show channel in channel list
|
| Parameters:  channel id(s), security code
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::setChanShow(const QString &cids, const QString &secCode)
{
   mInfo(tr("Show channel(s) %1 in channel list ...").arg(cids));

   QString req = QString("cmd=show_channel&cid=%1&protect_code=%2")
           .arg(cids).arg(secCode);

   PostRequest(Kartina::REQ_SETCHAN_SHOW, KARTINA_API_PATH "rule", req);
}

/*-----------------------------------------------------------------------------\
| Function:    getVodManager
|
| Author:      Jo2003
|
| Begin:       23.05.2012
|
| Description: request VOD manager data
|
| Parameters:  security code
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::getVodManager(const QString &secCode)
{
   mInfo(tr("Request VOD manager data ..."));

   QString req = QString("cmd=get_user_rates&protect_code=%1")
           .arg(secCode);

   PostRequest(Kartina::REQ_GET_VOD_MANAGER, KARTINA_API_PATH "vod_manage", req);
}

/*-----------------------------------------------------------------------------\
| Function:    setVodManager
|
| Author:      Jo2003
|
| Begin:       23.05.2012
|
| Description: set VOD manager data
|
| Parameters:  rules in form: "violence=hide&porn=pass", security code
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::setVodManager(const QString &rules, const QString &secCode)
{
   mInfo(tr("Set VOD manager data (%1) ...").arg(rules));

   QString req = QString("cmd=set_user_rates%1&protect_code=%2")
           .arg(rules).arg(secCode);

   PostRequest(Kartina::REQ_SET_VOD_MANAGER, KARTINA_API_PATH "vod_manage", req);
}

/*-----------------------------------------------------------------------------\
| Function:    addVodFav
|
| Author:      Jo2003
|
| Begin:       29.05.2012
|
| Description: add one video to favourites
|
| Parameters:  video id
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::addVodFav(int iVidID, const QString &secCode)
{
   mInfo(tr("Add VOD favourite (%1) ...").arg(iVidID));

   QString req = QString("id=%1").arg(iVidID);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   PostRequest(Kartina::REQ_ADD_VOD_FAV, KARTINA_API_PATH "vod_favadd", req);
}

/*-----------------------------------------------------------------------------\
| Function:    remVodFav
|
| Author:      Jo2003
|
| Begin:       29.05.2012
|
| Description: remove one video from favourites
|
| Parameters:  video id
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::remVodFav(int iVidID, const QString &secCode)
{
   mInfo(tr("Remove VOD favourite (%1) ...").arg(iVidID));

   QString req = QString("id=%1").arg(iVidID);

   if (secCode != "")
   {
      req += QString("&protect_code=%1").arg(secCode);
   }

   PostRequest(Kartina::REQ_REM_VOD_FAV, KARTINA_API_PATH "vod_favsub", req);
}

/*-----------------------------------------------------------------------------\
| Function:    getVodFav
|
| Author:      Jo2003
|
| Begin:       29.05.2012
|
| Description: request vod favourites
|
| Parameters:  --
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::getVodFav()
{
   mInfo(tr("Get VOD favourites (%1) ..."));
   GetRequest(Kartina::REQ_GET_VOD_FAV, KARTINA_API_PATH "vod_favlist");
}

/*-----------------------------------------------------------------------------\
| Function:    handleEndRequest (slot)
|
| Author:      Jo2003
|
| Begin:       Monday, January 04, 2010 16:14:52
|
| Description: sends messages if a request has finished ...
|
| Parameters:  request id (unused), error inidicator
|
| Returns:     --
\-----------------------------------------------------------------------------*/
void CKartinaClnt::handleEndRequest(int id, bool err)
{
   // is this our request ... ?
   if (id == iReq)
   {
      // close buffer device and open for read only...
      bufReq.close();
      bufReq.open(QIODevice::ReadOnly);

      // read all content ...
      baPageContent = bufReq.readAll();

      // close buffer device ...
      bufReq.close();

      if (!err)
      {
         mInfo(tr("Request #%1 (%2) done!").arg(id).arg(eReq));

         // unset cookie on logout ...
         if (eReq == Kartina::REQ_LOGOUT)
         {
             sCookie = "";
         }

         // send response ...
         emit sigHttpResponse(QString::fromUtf8(baPageContent.constData()), (int)eReq);
      }
      else
      {
         mErr(tr("Error in Request: %1!").arg(errorString()));

         // send error signal ...
         emit sigError(errorString());
      }

      // mark request as ended so the API is "free for use" again ...
      eReq = Kartina::REQ_UNKNOWN;
   }
}

/* -----------------------------------------------------------------\
|  Method: busy
|  Begin: 18.01.2010 / 16:32:33
|  Author: Jo2003
|  Description: is api busy ?
|
|  Parameters: --
|
|  Returns: true ==> busy
|          false ==> available
\----------------------------------------------------------------- */
bool CKartinaClnt::busy ()
{
   bool bRV;

   switch (state())
   {
   case QHttp::Unconnected:
   case QHttp::Connected:
      bRV = false;
      break;

   case QHttp::HostLookup:
   case QHttp::Connecting:
   case QHttp::Sending:
   case QHttp::Reading:
   case QHttp::Closing:
   default:
      bRV = true;
      break;
   }

   return (!bRV && (eReq == Kartina::REQ_UNKNOWN)) ? false : true;
}


/* -----------------------------------------------------------------\
|  Method: cookieSet
|  Begin: 24.01.2011 / 10:40
|  Author: Jo2003
|  Description: is cookie set?
|
|  Parameters: --
|
|  Returns: true ==> set
|          false ==> not set
\----------------------------------------------------------------- */
bool CKartinaClnt::cookieSet()
{
   return (sCookie != "") ? true : false;
}

/*=============================================================================\
|                                    History:
| ---------------------------------------------------------------------------
| 04.Jan.2010 - communication API for kartina.tv (inspired by conros)
\=============================================================================*/
