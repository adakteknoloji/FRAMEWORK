#!/bin/sh

set -e

if [ `uname -m` = x86_64 ] ; then
    ARCH=x86_64
    OBJDIR=LNX64
else
    ARCH=i386
    OBJDIR=LNX32
fi

echo "OBJDIR is $OBJDIR"

. ../../versiyon.sh


cp SPEC rpm.spec

sed "s/\%define name .*/%define name    $name/" rpm.spec > tmp_x
mv tmp_x rpm.spec

sed "s/BuildArch: .*/BuildArch : $ARCH/" rpm.spec > tmp_x
mv tmp_x rpm.spec


sed "s/\%define version .*/%define version $versiyon/" rpm.spec > tmp_x
mv tmp_x rpm.spec


BUILD_DIR=$name-$versiyon
rm -r -f $BUILD_DIR
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
chown -R root:root $BUILD_DIR

# Build install.tar.bz2 file
cd $BUILD_DIR
tar -cjf install.tar.bz2 opt
cd ..

rm -r -f $BUILD_DIR/opt
mkdir -p $BUILD_DIR/opt/CEREN/$ceren_subdir
mv $BUILD_DIR/install.tar.bz2 $BUILD_DIR/opt/CEREN/$ceren_subdir/install.tar.bz2
cp -f INSTALL.SH $BUILD_DIR/opt/CEREN/$ceren_subdir/install.sh
chown -R root:root $BUILD_DIR
tar -czf "$BUILD_DIR".tar.gz $BUILD_DIR
rm -r -f $BUILD_DIR

#Build rpm
mv -f $BUILD_DIR.tar.gz ~/rpm/SOURCES/
rpmbuild -bb rpm.spec

rm -f rpm.spec

mv -f ~/rpm/RPMS/$ARCH/$BUILD_DIR*.rpm .
