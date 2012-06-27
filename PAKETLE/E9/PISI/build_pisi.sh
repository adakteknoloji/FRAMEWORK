#!/bin/sh

set -e

OBJDIR=LNX32
ARCH=`uname -p`
if [ "$ARCH" = "x86_64" ] ; then
    OBJDIR=LNX64
fi

echo "OBJDIR is $OBJDIR"

. ../../versiyon.sh


BUILD_DIR=$name"-"$versiyon
name_versiyon=$name"-"$versiyon

rm -r -f $BUILD_DIR

mkdir -p $BUILD_DIR

mkdir -p $BUILD_DIR/opt/CEREN/$ceren_subdir

mkdir -p $BUILD_DIR/usr/share/applications/
cp -f ../K9.desktop $BUILD_DIR/usr/share/applications/

mkdir -p $BUILD_DIR/usr/share/icons/
cp -f ../K9_logo.png $BUILD_DIR/usr/share/icons/

cd $BUILD_DIR/opt/CEREN/$ceren_subdir

cp -f /PAKETLER/Qt/ceren_exec.sh .
cp -a -r -f /PAKETLER/Qt/$OBJDIR/* .
cp -a /CEREN/K9/$OBJDIR/K9 .
cd ../../../..
cp -f ../INSTALL.SH $BUILD_DIR/opt/CEREN/$ceren_subdir/install.sh
chown -R root:root $BUILD_DIR

tar -czf $name_versiyon.tar.gz $name_versiyon

sha1sum $name_versiyon.tar.gz > SHA1SUM
SHA1SUM=`cat SHA1SUM | cut -d ' ' -f 1`
rm -f SHA1SUM


##       pspec.xml hazirlaniyor

cp -f PSPEC.XML pspec.xml

sed "s/<Name>xx_name_xx<\/Name>/<Name>$name<\/Name>/" pspec.xml > tmp_x
mv tmp_x pspec.xml

sed "s/<Version>xx_versiyon_xx<\/Version>/<Version>$versiyon<\/Version>/" pspec.xml > tmp_x
mv tmp_x pspec.xml

sed "s/<Summary>xx_name_versiyon_xx<\/Summary>/<Summary>$name_versiyon<\/Summary>/" pspec.xml > tmp_x
mv tmp_x pspec.xml

sed "s/<Archive sha1sum=.*/<Archive sha1sum=\"$SHA1SUM\" type=\"targz\">file:\/\/\/PAKETLER\/K9\/PISI\/$name_versiyon.tar.gz<\/Archive>/" pspec.xml > tmp_x
mv tmp_x pspec.xml


##       pspec.xml hazirlandi





##       comar/package.py hazirlaniyor


sed "s/.*sh xxyyzz.*/    os.system (\"sh \/opt\/CEREN\/$ceren_subdir\/install.sh\")/" PACKAGE.PY > package.py

mkdir -p comar
mv package.py comar/


##       comar/package.py hazirlandi




rm -f $BUILD_DIR"*.pisi"

pisi build pspec.xml

rm -r -f $BUILD_DIR

rm -f $name_versiyon.tar.gz

rm -f pspec.xml

rm -r -f comar

rm -f /var/cache/pisi/archives/*
