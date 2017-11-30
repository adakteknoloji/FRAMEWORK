@echo off

set DEST_DIR=..\FRAMEWORK


mkdir %DEST_DIR%
mkdir %DEST_DIR%\MSVC

copy MSVC\ADAK.lib  %DEST_DIR%\MSVC

copy DESIGNER\MSVC\ADAK_DESIGNER.exe    %DEST_DIR%\MSVC

copy UPDATE_DB\MSVC\UPDATE_DB.exe       %DEST_DIR%\MSVC


copy QAdakDate\MSVC\QAdakDate.dll                %DEST_DIR%\MSVC
copy QCommaEdit\MSVC\QCommaEdit.dll              %DEST_DIR%\MSVC
copy QLimitedTextEdit\MSVC\QLimitedTextEdit.dll  %DEST_DIR%\MSVC
copy QSearchEdit\MSVC\QSearchEdit.dll            %DEST_DIR%\MSVC
