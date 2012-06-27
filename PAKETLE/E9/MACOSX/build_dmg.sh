#!/bin/sh


. ../versiyon.sh

root="K9"
echo "Building $root .."
rm -r -f $root

mkdir $root
mkdir -p $root/Applications/ADAK

cp -r -f ../../MACOSX/K9.app                $root/Applications/ADAK/
cp    -f K9.sh                              $root/Applications/ADAK/K9.app/Contents/MacOS/K9
cp -r -f ../Qt/MACOSX/*                     $root/Applications/ADAK/K9.app/
cd $root/Applications/ADAK/K9.app/lib/
ln -s libmysqlclient.18.dylib libmysqlclient.dylib
ln -s libmysqlclient.18.dylib libmysqlclient_r.18.dylib
ln -s libmysqlclient.18.dylib libmysqlclient_r.dylib
ln -s libpq.5.4.dylib libpq.5.dylib
cd ../../../../..

cp -r -f ../../MACOSX/K9.app/Contents/MacOS/K9 $root/Applications/ADAK/K9.app/lib/ADAK
cp -f PkgInfo                                  $root/Applications/ADAK/K9.app/Contents/
cp -f version.plist                            $root/Applications/ADAK/K9.app/Contents/
cp -f ../K9_logo.png                           $root/Applications/ADAK/K9.app/Contents/Resources/
rm  $root/Applications/ADAK/K9.app/Contents/Resources/empty.lproj
cp -r -f English.lproj                         $root/Applications/ADAK/K9.app/Contents/Resources/
echo "Done"


echo "Building $root"_"$versiyon.pkg .."
PM="/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker"

rm -r -f *.pkg
$PM --root $root --info Info.plist
echo "Done"
rm $root/Applications/ADAK/K9.app/lib/libmysqlclient.dylib
rm $root/Applications/ADAK/K9.app/lib/libmysqlclient_r.18.dylib
rm $root/Applications/ADAK/K9.app/lib/libmysqlclient_r.dylib
rm $root/Applications/ADAK/K9.app/lib/libpq.5.dylib
rm -r -f $root
mv *.pkg $root"_"$versiyon".pkg"


echo "Building adak_k9_$versiyon.dmg .."
rm -f *.dmg
hdiutil create -fs HFSX -layout SPUD -size 50m  adak_k9_"$versiyon".dmg -srcfolder $root"_"$versiyon.pkg -format UDRW -volname "K9 Ticari Paket" -quiet
rm -r -f *.pkg
rm -r -f ._*.dmg
echo "Done"



