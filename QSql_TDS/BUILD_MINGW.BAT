
qmake "INCLUDEPATH+=C:/ADAK/FRAMEWORK/FreeTDS/MINGW/include" "LIBS=-LC:/ADAK/FRAMEWORK/FreeTDS/MINGW/lib -lsybdb" tds.pro

mingw32-make clean all
mingw32-make install
