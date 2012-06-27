#!/bin/bash

set -e

if [ "$1" = "32" ] ; then
    ARCH=i386
    OBJDIR=LNX32
else
    ARCH=amd64
    OBJDIR=LNX64
fi

echo "OBJDIR is $OBJDIR"

. ../versiyon.sh

# Delete and create DIRECTORIES

BUILD_DIR=$name"_"$versiyon"_"$ARCH
name_versiyon=$name"_"$versiyon

rm -r -f $BUILD_DIR

mkdir -p $BUILD_DIR/DEBIAN
mkdir -p $BUILD_DIR/opt/ADAK/$adak_subdir



# Prepare control file

cp -f CONTROL control

sed -i "s/Package:.*/Package: $name/" control
sed -i "s/Version:.*/Version: $versiyon/" control
sed -i "s/Architecture:.*/Architecture: $ARCH/" control

mv -f control $BUILD_DIR/DEBIAN/


# Prepare postinst

cp -f POSTINST $BUILD_DIR/DEBIAN/postinst
chmod 755 $BUILD_DIR/DEBIAN/postinst

# Prepare Desktop Shortcut and ICON

mkdir -p $BUILD_DIR/usr/share/applications/
cp -f ../E9.desktop $BUILD_DIR/usr/share/applications/

mkdir -p $BUILD_DIR/usr/share/icons/
cp -f ../E9_logo.png $BUILD_DIR/usr/share/icons/

pwd
echo ""

cp -a -f         ../../Qt/adak_exec.sh                     $BUILD_DIR/opt/ADAK/$adak_subdir/
mkdir $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libphonon.so*        $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtCLucene.so*     $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtCore.so*        $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtDBus.so*        $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtGui.so*         $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtNetwork.so*     $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtOpenGL.so*      $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtSql.so*         $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtSvg.so*         $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtWebKit.so*      $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtXml.so*         $BUILD_DIR/opt/ADAK/$adak_subdir/lib/
cp -a -f         ../../Qt/$OBJDIR/lib/libQtXmlPatterns.so* $BUILD_DIR/opt/ADAK/$adak_subdir/lib/

mkdir $BUILD_DIR/opt/ADAK/$adak_subdir/plugins/
cp -a -f -r      ../../Qt/$OBJDIR/plugins/imageformats     $BUILD_DIR/opt/ADAK/$adak_subdir/plugins/
mkdir $BUILD_DIR/opt/ADAK/$adak_subdir/plugins/sqldrivers
cp -a -f         ../../../QSql_*/$OBJDIR/*                 $BUILD_DIR/opt/ADAK/$adak_subdir/lib/

mv               $BUILD_DIR/opt/ADAK/$adak_subdir/lib/libqsql*.so $BUILD_DIR/opt/ADAK/$adak_subdir/plugins/sqldrivers/

cp -a -f         ../../../../E9/$OBJDIR/E9                 $BUILD_DIR/opt/ADAK/$adak_subdir/
strip --strip-unneeded $BUILD_DIR/opt/ADAK/$adak_subdir/E9
cp -f ../../FILES/INSTALL.SH $BUILD_DIR/opt/ADAK/$adak_subdir/install.sh


rm -f $BUILD_DIR"*.deb"

cd $BUILD_DIR
chown -R root:root *
dpkg-deb -b -Zbzip2 -z9 . ../$BUILD_DIR".deb"
cd ..


rm -r -f $BUILD_DIR
