#!/bin/sh

#
#
#         Windows Versiyonu (32 bit)
#
#

. ../versiyon.sh

exe_filename="adak_k9_"$versiyon".exe"

sed "s/!define PRODUCT_VERSION \"x.yy\"/!define PRODUCT_VERSION \"$versiyon\"/" SCRIPT.nsi > script.nsi

cp ../LISANS.txt lisans.txt

strip --strip-unneeded ../../WIN32/K9.exe
wine "C:\Program Files\NSIS\makensis.exe" script.nsi
mv Setup.exe $exe_filename

rm script.nsi lisans.txt
