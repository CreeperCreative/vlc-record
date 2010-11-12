/*********************** Information *************************\
| $HeadURL$
|
| Author: Joerg Neubert
|
| Begin: 19.01.2010 / 16:23:07
|
| Last edited by: $Author$
|
| $Id$
\*************************************************************/
#ifndef __011910__VERSION_INFO_H
   #define __011910__VERSION_INFO_H

#ifdef WIN32
   #define MY_BUILD_SYS "Windows"
#else
   #define MY_BUILD_SYS "Linux (x86)"
#endif

#define __MY__VERSION__ \
   "1.06 - " MY_BUILD_SYS ", built at " __DATE__

#endif /* __011910__VERSION_INFO_H */
/************************* History ***************************\
| 2010-01-19: Version 1.00
| - missing comments added
| - prepare for linux build
| - reload epg only if we are at current day
| 2010-01-20: Version 1.01
| - try to fix time offset problem
| 2010-01-20: Version 1.05
| - add advanced epg navbar
| 2010-01-21: Version 1.06
| - create / translate epg navbar using only one function
| - double click on channel list starts channel play
\*************************************************************/
