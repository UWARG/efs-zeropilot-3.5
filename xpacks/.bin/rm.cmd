@ECHO off
GOTO start
:find_dp0
SET dp0=%~dp0
EXIT /b
:start
SETLOCAL
CALL :find_dp0
"C:\Users\uzayr\AppData\Roaming\xPacks\@xpack-dev-tools\windows-build-tools\4.4.1-2.1\.content\bin\rm.exe"   %*
