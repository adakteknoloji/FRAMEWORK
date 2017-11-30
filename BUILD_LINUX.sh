#! /bin/bash

set -e


DIZINLER="UTILS SQL ADAK_STD QCommaEdit QSearchEdit QLimitedTextEdit QAdakDate ODS_IO KERNEL GRUP_KERNEL MENU_KERNEL EMAIL SMS REPORT_KERNEL BELGE_KERNEL TASARIMCI LOGIN YONETIM"


for dizin in $DIZINLER
do
	echo "\n\n$dizin DERLENIYOR\n\n"
	cd $dizin
	qmake
	make -j 8
	cd ..
	echo "\n\n$dizin DERLENDI\n\n"
done


qmake
make

DIZINLER="UPDATE_DB DESIGNER"
for dizin in $DIZINLER
    do
    echo "\n\n$dizin DERLENIYOR\n\n"
    cd $dizin
    qmake
    make -j 8
    cd ..
    echo "\n\n$dizin DERLENDI\n\n"
done
