:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
:: Contact: Qt Software Information (qt-info@nokia.com)
::
:: This file is part of the Windows installer of the Qt Toolkit.
::
:: Commercial Usage
:: Licensees holding valid Qt Commercial licenses may use this file in
:: accordance with the Qt Commercial License Agreement provided with the
:: Software or, alternatively, in accordance with the terms contained in
:: a written agreement between you and Nokia.
::
::
:: GNU General Public License Usage
:: Alternatively, this file may be used under the terms of the GNU
:: General Public License versions 2.0 or 3.0 as published by the Free
:: Software Foundation and appearing in the file LICENSE.GPL included in
:: the packaging of this file.  Please review the following information
:: to ensure GNU General Public Licensing requirements will be met:
:: http://www.fsf.org/licensing/licenses/info/GPLv2.html and
:: http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
:: exception, Nokia gives you certain additional rights. These rights
:: are described in the Nokia Qt GPL Exception version 1.3, included in
:: the file GPL_EXCEPTION.txt in this package.
::
:: Qt for Windows(R) Licensees
:: As a special exception, Nokia, as the sole copyright holder for Qt
:: Designer, grants users of the Qt/Eclipse Integration plug-in the
:: right for the Qt/Eclipse Integration to link to functionality
:: provided by Qt Designer and its related libraries.
::
:: If you are unsure which license is appropriate for your use, please
:: contact the sales department at qt-sales@nokia.com.
::
:: This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
:: WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
call :%1 %2
goto END

:destDir
  if exist "%IWMAKE_OUTDIR%\%~1" rd /S /Q %IWMAKE_OUTDIR%\%~1
goto :eof

:dir
  if exist "%IWMAKE_ROOT%\%~1" rd /S /Q %IWMAKE_ROOT%\%~1
goto :eof

:dirAbs
  if exist "%~1"  rd /S /Q %~1
goto :eof

:file
  del /Q /F %IWMAKE_OUTDIR%\%~1 >> %IWMAKE_LOGFILE% 2>&1
  exit /b 0
goto :eof

:files
  del /S /Q /F %IWMAKE_OUTDIR%\%~1 >> %IWMAKE_LOGFILE% 2>&1
  exit /b 0
goto :eof

:line
  for /F "tokens=1*" %%m in ("%~1") do set IWMAKE_TMP=%%~m& set IWMAKE_TMP2=%%~n
  if exist "%IWMAKE_ROOT%\tmp_line.txt" del /Q /F "%IWMAKE_ROOT%\tmp_line.txt" >> %IWMAKE_LOGFILE%
  type "%IWMAKE_ROOT%\%IWMAKE_TMP%" | find /V "%IWMAKE_TMP2%" >> "%IWMAKE_ROOT%\tmp_line.txt"
  xcopy /Y /Q /R %IWMAKE_ROOT%\tmp_line.txt %IWMAKE_ROOT%\%IWMAKE_TMP% >> %IWMAKE_LOGFILE%
goto :eof

:END
