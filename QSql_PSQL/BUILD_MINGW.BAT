rem postgresql-9.1.4-1-windows-binaries.zip c:\pgsql ' e unzip edilmeli


qmake "INCLUDEPATH+=C:/pgsql/include" "LIBS+=-LC:/pgsql/lib" psql.pro
mingw32-make clean all
mingw32-make install

