
cd /opt/CEREN/lib
LINKS="libphonon.so libphonon.so.4 libphonon.so.4.4 "
LINKS=$LINKS"libQtCore.so libQtCore.so.4 libQtCore.so.4.7 "
LINKS=$LINKS"libQtDBus.so libQtDBus.so.4 libQtDBus.so.4.7 "
LINKS=$LINKS"libQtGui.so libQtGui.so.4 libQtGui.so.4.7 "
LINKS=$LINKS"libQtNetwork.so libQtNetwork.so.4 libQtNetwork.so.4.7 "
LINKS=$LINKS"libQtSql.so libQtSql.so.4 libQtSql.so.4.7 "
LINKS=$LINKS"libQtSvg.so libQtSvg.so.4 libQtSvg.so.4.7 "
LINKS=$LINKS"libQtWebKit.so libQtWebKit.so.4 libQtWebKit.so.4.7 "
LINKS=$LINKS"libQtXmlPatterns.so libQtXmlPatterns.so.4 libQtXmlPatterns.so.4.7 "
LINKS=$LINKS"libQtXml.so libQtXml.so.4 libQtXml.so.4.7 "

for link in $LINKS;
do
    if [ -e $link ] ; then
        unlink $link
        rm -f $link
    fi
done


ln -s libphonon.so.4.4.0         libphonon.so
ln -s libphonon.so.4.4.0         libphonon.so.4
ln -s libphonon.so.4.4.0         libphonon.so.4.4

ln -s libQtCore.so.4.7.4         libQtCore.so
ln -s libQtCore.so.4.7.4         libQtCore.so.4
ln -s libQtCore.so.4.7.4         libQtCore.so.4.7

ln -s libQtDBus.so.4.7.4         libQtDBus.so
ln -s libQtDBus.so.4.7.4         libQtDBus.so.4
ln -s libQtDBus.so.4.7.4         libQtDBus.so.4.7

ln -s libQtGui.so.4.7.4          libQtGui.so
ln -s libQtGui.so.4.7.4          libQtGui.so.4
ln -s libQtGui.so.4.7.4          libQtGui.so.4.7

ln -s libQtNetwork.so.4.7.4      libQtNetwork.so
ln -s libQtNetwork.so.4.7.4      libQtNetwork.so.4
ln -s libQtNetwork.so.4.7.4      libQtNetwork.so.4.7

ln -s libQtSql.so.4.7.4          libQtSql.so
ln -s libQtSql.so.4.7.4          libQtSql.so.4
ln -s libQtSql.so.4.7.4          libQtSql.so.4.7

ln -s libQtSvg.so.4.7.4          libQtSvg.so
ln -s libQtSvg.so.4.7.4          libQtSvg.so.4
ln -s libQtSvg.so.4.7.4          libQtSvg.so.4.7

ln -s libQtWebKit.so.4.7.4       libQtWebKit.so
ln -s libQtWebKit.so.4.7.4       libQtWebKit.so.4
ln -s libQtWebKit.so.4.7.4       libQtWebKit.so.4.7

ln -s libQtXmlPatterns.so.4.7.4  libQtXmlPatterns.so
ln -s libQtXmlPatterns.so.4.7.4  libQtXmlPatterns.so.4
ln -s libQtXmlPatterns.so.4.7.4  libQtXmlPatterns.so.4.7

ln -s libQtXml.so.4.7.4          libQtXml.so
ln -s libQtXml.so.4.7.4          libQtXml.so.4
ln -s libQtXml.so.4.7.4          libQtXml.so.4.7



#########
## Make library links


cd /opt/CEREN/lib

ln -s libpng12.so.0.27.0         libpng12.so
ln -s libpng12.so.0.27.0         libpng12.so.0

ln -s libmysqlclient.so.15.0.0   libmysqlclient.so.15
ln -s libmysqlclient.so.15       libmysqlclient.so

ln -s libmysqlclient_r.so.15.0.0 libmysqlclient_r.so.15
ln -s libmysqlclient_r.so.15     libmysqlclient_r.so

