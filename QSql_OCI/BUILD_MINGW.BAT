rem instantclient-basic-nt-11.2.0.2.0.zip instantclient-sdk-nt-11.2.0.2.0.zip C:\instantclient_11_2 dizinine unzip edilmeli

qmake "INCLUDEPATH+=C:/instantclient_11_2/sdk/include" "LIBS+=-LC:/instantclient_11_2/sdk/lib/msvc" oci.pro
mingw32-make clean all
mingw32-make install
