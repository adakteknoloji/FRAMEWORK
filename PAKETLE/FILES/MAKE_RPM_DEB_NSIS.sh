# !/bin/sh

#set -e

DIZINLER="K9 SYS"

for dizin in $DIZINLER;
do
	cd $dizin/RPM
	sh make.sh
	cd ../DEB
	sh make.sh;sh make.sh 32
	cd ../NSIS
	#cd ../WIN32
	sh make.sh
	cd ../..
done
