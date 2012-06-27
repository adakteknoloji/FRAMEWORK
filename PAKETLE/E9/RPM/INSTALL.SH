#!/bin/sh

cd /
tar -jxvf /opt/CEREN/K9/install.tar.bz2 > /dev/null

chmod 777 /opt/CEREN

xdg-icon-resource install --novendor --size 64 --mode system /usr/share/icons/K9_logo.png
xdg-desktop-icon install --novendor /usr/share/applications/K9.desktop

rm -f /opt/CEREN/K9/install.tar.bz2
rm -f /opt/CEREN/K9/install.sh
