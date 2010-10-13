/*********************** Information *************************\
| $HeadURL$
|
| Author: Joerg Neubert
|
| Begin: 18.01.2010 / 16:07:29
|
| Last edited by: $Author$
|
| $Id$
\*************************************************************/
#include "cepgbrowser.h"

// log file functions ...
extern CLogFile VlcLog;

/* -----------------------------------------------------------------\
|  Method: CEpgBrowser / constructor
|  Begin: 18.01.2010 / 16:07:59
|  Author: Joerg Neubert
|  Description: create object, init values
|
|  Parameters: pointer to parent widget
|
|  Returns: --
\----------------------------------------------------------------- */
CEpgBrowser::CEpgBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
   iTimeShift = 0;
   iCid       = 0;
   sLogoDir   = "";
}

/* -----------------------------------------------------------------\
|  Method: DisplayEpg
|  Begin: 18.01.2010 / 16:08:35
|  Author: Joerg Neubert
|  Description: display EPG entries
|
|  Parameters: list of entries, channel name, channel id,
|              timestamp, archiv flag
|
|  Returns: --
\----------------------------------------------------------------- */
void CEpgBrowser::DisplayEpg(QVector<cparser::SEpg> epglist,
                             const QString &sName, int iChanID, uint uiGmt,
                             bool bHasArchiv)
{
   QString   sRows   = "", sRow, sProgCell, sHtmlDoc, sStartTime, sHeadLine;
   QDateTime dtStartThis, dtStartNext;
   bool      bMark;

   // store channel id ...
   iCid = iChanID;

   // create headline (table head) ...
   sHeadLine = QString("%1 - %2")
               .arg(sName)
               .arg(QDateTime::fromTime_t(uiGmt).toString("dd. MMM. yyyy"));

   sRow = TR_HEAD;
   sRow.replace(TMPL_HEAD, sHeadLine);
   sRows += sRow;

   for (int i = 0; i < epglist.size(); i ++)
   {
      bMark       = false;
      sProgCell   = "";
      dtStartThis = QDateTime::fromTime_t(epglist[i].uiGmt + iTimeShift * 3600);

      // find out if we should mark the time ...
      if ((i + 1) < epglist.size())
      {
         dtStartNext = QDateTime::fromTime_t(epglist[i + 1].uiGmt + iTimeShift * 3600);
         bMark       = NowRunning(dtStartThis, dtStartNext);
      }
      else
      {
         bMark       = NowRunning(dtStartThis);
      }

      if (bMark)
      {
         sRow      = TR_TMPL_ACTUAL;
         sProgCell = "<a name='nowPlaying' />";
      }
      else
      {
         sRow = (i % 2) ? TR_TMPL_B : TR_TMPL_A;
      }

      sProgCell += epglist[i].sName;

      if (epglist[i].sDescr != "")
      {
         sProgCell += QString("<br /><span style='color: #666'>%1</span>").arg(epglist[i].sDescr);
      }

      sStartTime = dtStartThis.toString("hh:mm");

      // archiv available ...
      if (bHasArchiv)
      {
         // only show archiv links if this show already has ended ...
         if (ArchivAvailable(epglist[i].uiGmt, ((i + 1) < epglist.size()) ? epglist[i + 1].uiGmt : 0))
         {
            QString sArchivLinks = QString("<hr /><b>%1:</b> &nbsp;")
                                   .arg(tr("Ar."));

            // play ...
            sArchivLinks += QString("<a href='vlc-record?action=archivplay&cid=%1&gmt=%2'>"
                                    "<img src=':png/play' width='16' height='16' /></a>&nbsp;")
                                    .arg(iChanID).arg(epglist[i].uiGmt);

            // record ...
            sArchivLinks += QString("<a href='vlc-record?action=archivrec&cid=%1&gmt=%2'>"
                                    "<img src=':png/record' width='16' height='16' /></a>")
                                    .arg(iChanID).arg(epglist[i].uiGmt);

            sStartTime += sArchivLinks;
         }
      }

      sRow.replace(TMPL_PROG, sProgCell);
      sRow.replace(TMPL_TIME, sStartTime);

      sRows += sRow;
   }

   sHtmlDoc = EPG_TMPL;
   sHtmlDoc.replace(TMPL_ROWS, sRows);

   clear();
   setHtml(sHtmlDoc);
   scrollToAnchor("nowPlaying");
}

/* -----------------------------------------------------------------\
|  Method: NowRunning
|  Begin: 18.01.2010 / 16:09:56
|  Author: Joerg Neubert
|  Description: check if given show is now running
|
|  Parameters: this shows start time, next shows start time
|
|  Returns: true ==> running
|          false ==> not running
\----------------------------------------------------------------- */
bool CEpgBrowser::NowRunning (const QDateTime &startThis, const QDateTime &startNext)
{
   bool bNowRunning = false;

   // now given, later not given ...
   if (startThis.isValid() && !startNext.isValid())
   {
      int diff = QDateTime::currentDateTime().toTime_t() - startThis.toTime_t();

      // mark this show as running, if start wasn't more
      // than 3 hours ago ...
      if ((diff >= 0) && (diff < (3600 * 3))) // 3 hours
      {
         bNowRunning = true;
      }
   }
   // now and later given ...
   else if (startThis.isValid() && startNext.isValid())
   {
      if ((QDateTime::currentDateTime() >= startThis)
         && (QDateTime::currentDateTime() <= startNext))
      {
         bNowRunning = true;
      }
   }

   return bNowRunning;
}

/* -----------------------------------------------------------------\
|  Method: ArchivAvailable
|  Begin: 18.01.2010 / 16:11:12
|  Author: Joerg Neubert
|  Description: check if archiv is avalable for given show
|
|  Parameters: timestamp for this show, timestamp for next show
|
|  Returns: true ==> archiv available
|          false ==> not available
\----------------------------------------------------------------- */
bool CEpgBrowser::ArchivAvailable(uint uiThisShow, uint uiNextShow)
{
   bool      bArchiv   = false;
   uint      uiCompare = (uiNextShow) ? uiNextShow : uiThisShow;
   QDateTime AnyTime   = QDateTime::fromTime_t(uiCompare);

   // next show already started ...
   if (AnyTime < QDateTime::currentDateTime())
   {
      // this means:
      // former show should be already in archiv ...
      bArchiv = true;
   }

   return bArchiv;
}

/************************* History ***************************\
| $Log$
\*************************************************************/
