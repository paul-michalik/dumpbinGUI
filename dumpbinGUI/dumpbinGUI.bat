@echo off

setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
start "dumpbinGUI.exe" dumpbinGUI.exe
exit /b %errorlevel%
endlocal