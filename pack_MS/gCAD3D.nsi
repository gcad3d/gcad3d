; gCAD3D-Installation-script.           RF 2005-06-12
; 2012-01-17 gCAD3D.bat RF
; 2011-02-02 %osVer%  RF
; 2010-06-17 binMS32  RF

; Programs go into: (binMS32 ..)
; "%ProgramFiles%\gCAD3D\"

; Data go into:  (dat, tmp ..)
; "%APPDATA%\gCAD3D\"



; set /p swVers= < gcad_version
; set osVer=win32
!define PRODUCT_VERSION $%swVers%-$%osVer%


!define PRODUCT_NAME "gCAD3D"
!define PRODUCT_PUBLISHER "Franz Reiter"
!define PRODUCT_WEB_SITE "http://www.gCAD3D.org"

!define SrcDir "."

; inputDirectory ReadOnly
; geht nach INSTDIR (\Programme\)
!define PrgDir "$%TEMP%\gCAD3D\Prg"

; inputDirectory FullAccess
; geht nach APPDATA (\Dokumente und Einstellungen\%USER%)
!define DatDir "$%TEMP%\gCAD3D\Dat"


!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\gCAD3D.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"




SetCompressor LZMA   ; We will use LZMA for best compression


;================================================================
; MUI Settings
!include "MUI.nsh"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
!define MUI_ABORTWARNING
!define MUI_ICON "${SrcDir}\gCAD3D.ico"
!define MUI_UNICON "${SrcDir}\unInstall.ico"


; Welcome page
!insertmacro MUI_PAGE_WELCOME

; License page
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"

; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\gCAD3D.bat"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"



;================================================================
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "gCAD3D-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\gCAD3D"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show


;================================================================
Section "Hauptgruppe" SEC01

  ; copy programfiles
  SetOutPath "$INSTDIR"
  File /r "${PrgDir}\*.*"

  ; copy datafiles
  SetOutPath "$APPDATA"
  File /r "${DatDir}\*.*"

  CreateDirectory "$SMPROGRAMS\gCAD3D"

  CreateShortCut "$SMPROGRAMS\gCAD3D\gCAD3D.lnk" "$INSTDIR\gCAD3D.bat"\
    "" "$INSTDIR\gCAD3D.ico"

  CreateShortCut "$DESKTOP\gCAD3D.lnk" "$INSTDIR\gCAD3D.bat"\
    "" "$INSTDIR\gCAD3D.ico"
  ;0 SW_SHOWMINIMIZED   2022-07-01 removed

SectionEnd




;================================================================
Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\gCAD3D\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\gCAD3D\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd




;================================================================
Section -Post


; write startFile
FileOpen $0 $INSTDIR\gCAD3D.bat w
FileWrite $0 "@ECHO OFF$\r$\n"
FileWrite $0 "set gcad_dir_bin=$INSTDIR\binMS32\$\r$\n"
FileWrite $0 "set gcad_dir_doc=$INSTDIR\doc\$\r$\n"
FileWrite $0 "set gcad_dir_local=$APPDATA\$\r$\n"
FileWrite $0 "REM set gcad_dir_dev=$\r$\n"
FileWrite $0 "if .debug==.%1 ($\r$\n"
FileWrite $0 "windbg $\"%gcad_dir_bin%\gCAD3D.exe$\"$\r$\n"
FileWrite $0 ") else ($\r$\n"
FileWrite $0 "$\"%gcad_dir_bin%gCAD3D.exe$\" %*$\r$\n"
FileWrite $0 ")$\r$\n"
FileClose $0

; write cacls-File
ExecWait '"$INSTDIR\gCAD3D.bat"'
FileOpen $0 $INSTDIR\install1.bat w
FileWrite $0 "@ECHO OFF$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\tmp$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\dat$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\cfg$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\prg$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\ctlg$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\Profile$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\Schrau$\" /t /e /c /p everyone:f$\r$\n"
FileWrite $0 "cacls $\"$APPDATA\symEl1$\" /t /e /c /p everyone:f$\r$\n"
FileClose $0

; fix cacls
ExecWait '"$INSTDIR\install1.bat"'


  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\binMS32\gCAD3D.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\binMS32\gCAD3D.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

SectionEnd


/*
Function .onInit
  MessageBox MB_OK "Inst. to ${INSTDIR}"
FunctionEnd
*/




;================================================================
Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Möchten Sie $(^Name) und alle seine Komponenten deinstallieren?$\n$\rThis will uninstall $(^Name) from your system" IDYES +2
  Abort
FunctionEnd



Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) wurde erfolgreich deinstalliert."
FunctionEnd



Section Uninstall

  Delete "$SMPROGRAMS\gCAD3D\Uninstall.lnk"
  Delete "$SMPROGRAMS\gCAD3D\Website.lnk"
  Delete "$DESKTOP\gCAD3D.lnk"
  Delete "$SMPROGRAMS\gCAD3D\gCAD3D.lnk"

  RMDir "$SMPROGRAMS\gCAD3D"

  RMDir /r /REBOOTOK "$INSTDIR"

  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Delete also datafiles ?" /SD IDYES IDNO L9
  RMDir /r "$APPDATA\gCAD3D"

L9:
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true

SectionEnd



;================= EOF =======================
