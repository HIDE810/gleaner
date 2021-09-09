@echo off
:run
gleaner
echo.
echo Continue?
choice
if %errorlevel%==1 goto run