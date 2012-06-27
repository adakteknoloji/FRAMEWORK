#!/bin/sh

WORKDIR=/home/ccvural/

mkdir -p $WORKDIR/VERSIYON/k9
mv -f K9/*/ceren*.rpm  $WORKDIR/VERSIYON/k9
mv -f K9/*/ceren*.pisi $WORKDIR/VERSIYON/k9
mv -f K9/*/ceren*.deb  $WORKDIR/VERSIYON/k9
mv -f K9/*/ceren*.exe  $WORKDIR/VERSIYON/k9
mv -f K9/*/ceren*.dmg  $WORKDIR/VERSIYON/k9

mkdir -p $WORKDIR/VERSIYON/k9_sys
mv -f SYS/*/ceren*.rpm  $WORKDIR/VERSIYON/k9_sys
mv -f SYS/*/ceren*.pisi $WORKDIR/VERSIYON/k9_sys
mv -f SYS/*/ceren*.deb  $WORKDIR/VERSIYON/k9_sys
mv -f SYS/*/ceren*.exe  $WORKDIR/VERSIYON/k9_sys
mv -f SYS/*/ceren*.dmg  $WORKDIR/VERSIYON/k9_sys
