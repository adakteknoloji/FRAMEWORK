#!/bin/bash

set -e

if ! [ "$USER" == "root" ]
then
    echo "Paketi olusturabilmek icin 'root' yetkisine sahip olmalisiniz."
    exit 1
fi 
PKG_ARCH=$1
if [ "$PKG_ARCH" == "" ]
then
    PKG_ARCH=32
    if [ "`uname -m`" == "x86_64" ]
    then
        PKG_ARCH=64
    fi
fi

name="adak-e9"
adak_subdir="E9"

. ./build_deb.sh $PKG_ARCH
