@echo off
REM Launch Bing Maps Plugin Preview
REM This script runs the QML preview using qmlscene

echo Starting Bing Maps Plugin Preview...
echo.

REM Set Qt environment
set QTDIR=C:\Qt\5.15.2\msvc2019_64
set PATH=%QTDIR%\bin;%PATH%
set QML2_IMPORT_PATH=%QTDIR%\qml
set QT_PLUGIN_PATH=%QTDIR%\plugins

REM Enable Qt debug output
set QT_LOGGING_RULES=qt.location*=true

echo Qt Directory: %QTDIR%
echo Plugin Path: %QT_PLUGIN_PATH%
echo.

REM Run qmlscene
qmlscene preview.qml

pause
