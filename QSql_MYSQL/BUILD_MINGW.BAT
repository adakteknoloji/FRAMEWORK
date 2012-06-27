rem mysql-5.5.25-win32 C:\ ' ta yuklu olmali

qmake "INCLUDEPATH+=C:/mysql-5.5.25-win32/include" "LIBS+=C:/mysql-5.5.25-win32/lib/libmysql.lib" mysql.pro
mingw32-make clean all
mingw32-make install
