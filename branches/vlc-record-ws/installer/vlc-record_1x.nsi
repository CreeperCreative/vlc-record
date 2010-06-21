;-------------------------------------------------------
; Include Modern UI

  !include "MUI2.nsh"

;-------------------------------------------------------
; Include defines ...
  !include "defines.nsh"

;-------------------------------------------------------
; General

  ;Name and file
  Name "${APPNAME} Classic ${VER_CLASSIC}"
  OutFile "${PACKAGES}\${APPNAME}-${VER_CLASSIC}-win-x86-setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${APPNAME} Classic"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\${APPNAME} Classic" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

  SetCompressor /FINAL /SOLID lzma

;-------------------------------------------------------
; Interface Settings
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\nsis.bmp" ; optional
  !define MUI_ABORTWARNING

;-------------------------------------------------------
; what to run when finished ... ?
  !define MUI_FINISHPAGE_RUN "$INSTDIR\vlc-record.exe"

;-------------------------------------------------------
; Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "gpl-3.0.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;-------------------------------------------------------
; Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_RESERVEFILE_LANGDLL

;-------------------------------------------------------
; Installer Sections for vlc-record
Section "VLC-Record" SecInst
  SectionIn RO
  SetOutPath "$INSTDIR"
  File "${SRCDIR}\release\vlc-record.exe"
  File "${QTLIBS}\libgcc_s_dw2-1.dll"
  File "${QTLIBS}\mingwm10.dll"

  SetOutPath "$INSTDIR\language"
  File "${SRCDIR}\lang_de.qm"
  File "${SRCDIR}\lang_ru.qm"

  SetOutPath "$INSTDIR\modules"
  File "${SRCDIR}\modules\1_vlc-player.mod"
  File "${SRCDIR}\modules\2_MPlayer.mod"
  File "${SRCDIR}\modules\3_vlc-mp4.mod"
  File "${SRCDIR}\modules\4_vlc-player-avi.mod"
  File "${SRCDIR}\modules\7_vlc-mpeg2.mod"

SectionEnd

;-------------------------------------------------------
; Installer Sections for qt libraries
Section "qt Framework" SecQt
   SetOutPath "$INSTDIR"
   File "${QTLIBS}\QtCore4.dll"
   File "${QTLIBS}\QtSql4.dll"
   FILE "${QTLIBS}\QtGui4.dll"
   FILE "${QTLIBS}\QtNetwork4.dll"

   SetOutPath "$INSTDIR\imageformats"
   File /r "${QTLIBS}\imageformats\*.dll"

   SetOutPath "$INSTDIR\sqldrivers"
   File /r "${QTLIBS}\sqldrivers\*.dll"
SectionEnd

;-------------------------------------------------------
; start menu entries 
Section "Start Menu Entries" SecStart
	CreateDirectory "$SMPROGRAMS\${APPNAME} Classic"
	CreateShortCut "$SMPROGRAMS\${APPNAME} Classic\${APPNAME} Classic.lnk" "$INSTDIR\vlc-record.exe"
	CreateShortCut "$SMPROGRAMS\${APPNAME} Classic\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;-------------------------------------------------------
; desktop shortcut ...
Section /o "Desktop Shortcut" SecDesktop
	CreateShortCut "$DESKTOP\${APPNAME} Classic.lnk" "$INSTDIR\vlc-record.exe"
SectionEnd

;-------------------------------------------------------
; write uninstall stuff ...
Section -FinishSection
  ;store installation folder ...
  WriteRegStr HKLM "Software\${APPNAME} Classic" "" "$INSTDIR"
	
  ; create uninstall entries in registry ...
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME} Classic" "DisplayName" "${APPNAME} Classic"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME} Classic" "UninstallString" "$INSTDIR\uninstall.exe"

  ; write uninstaller ...
  WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

;-------------------------------------------------------
; Descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecInst} "The vlc-record executable, the language files and player modules."
	!insertmacro MUI_DESCRIPTION_TEXT ${SecQt} "The Qt framework. Only disable this section if you have already installed the Qt framework and have set the QTDIR environment variable."
	!insertmacro MUI_DESCRIPTION_TEXT ${SecStart} "Creates a start menu entry for ${APPNAME}"
	!insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} "Creates a desktop shortcut for ${APPNAME}"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;-------------------------------------------------------
; Uninstaller Section Qt ...
Section "un.Qt"
  ; delete Qt framework ...
  Delete "$INSTDIR\imageformats\*.dll"
  Delete "$INSTDIR\sqldrivers\*.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  RMDir  "$INSTDIR\imageformats"
  RMDir  "$INSTDIR\sqldrivers"
SectionEnd

;-------------------------------------------------------
; Uninstaller Section vlc-record ...
Section "un.Program"
  ; delete installed language files ...
  Delete "$INSTDIR\language\lang_de.qm"
  Delete "$INSTDIR\language\lang_ru.qm"

  ; delete installed module files ...
  Delete "$INSTDIR\modules\1_vlc-player.mod"
  Delete "$INSTDIR\modules\2_MPlayer.mod"
  Delete "$INSTDIR\modules\3_vlc-mp4.mod"
  Delete "$INSTDIR\modules\4_vlc-player-avi.mod"
  Delete "$INSTDIR\modules\7_vlc-mpeg2.mod"

  ; delete directories ...
  RMDir  "$INSTDIR\modules"
  RMDir  "$INSTDIR\language"

  ; delete vlc-record itself ...
  Delete "$INSTDIR\vlc-record.exe"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"

SectionEnd

;-------------------------------------------------------
; Remove from registry...
Section "un.registry"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME} Classic"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME} Classic"
SectionEnd

;-------------------------------------------------------
; Delete Shortcuts
Section "un.Shortcuts"
	Delete "$DESKTOP\${APPNAME} Classic.lnk"
	Delete "$SMPROGRAMS\${APPNAME} Classic\${APPNAME} Classic.lnk"
	Delete "$SMPROGRAMS\${APPNAME} Classic\Uninstall.lnk"
	RMDir  "$SMPROGRAMS\${APPNAME} Classic"
SectionEnd

;-------------------------------------------------------
; make final cleaning ...
Section "un.FinalCleaning"
	; delete uninstaller ...
  Delete "$INSTDIR\Uninstall.exe"

  ; delete install dir ...
	RMDir "$INSTDIR"
SectionEnd
