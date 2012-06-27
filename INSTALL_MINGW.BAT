@echo off

set DEST_DIR=..\E9\ADAK


mkdir %DEST_DIR%
mkdir %DEST_DIR%\MINGW


copy MINGW\libADAK.a  %DEST_DIR%\MINGW

copy DESIGNER\MINGW\ADAK_DESIGNER.exe   %DEST_DIR%\MINGW

copy UPDATE_DB\MINGW\UPDATE_DB.exe      %DEST_DIR%\MINGW


copy QAdakDate\MINGW\QAdakDate.dll                %DEST_DIR%\MINGW
copy QCommaEdit\MINGW\QCommaEdit.dll              %DEST_DIR%\MINGW
copy QLimitedTextEdit\MINGW\QLimitedTextEdit.dll  %DEST_DIR%\MINGW
copy QSearchEdit\MINGW\QSearchEdit.dll            %DEST_DIR%\MINGW
