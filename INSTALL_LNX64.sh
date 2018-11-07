#! /bin/bash

DEST_DIR=../FRAMEWORK


mkdir -p $DEST_DIR

mkdir -p $DEST_DIR/LNX64

cp -f LNX64/libADAK.a  $DEST_DIR/LNX64/

cp -f DESIGNER/LNX64/ADAK_DESIGNER      $DEST_DIR/LNX64/

cp -f UPDATE_DB/LNX64/UPDATE_DB      $DEST_DIR/LNX64/

cp -f -a QAdakDate/LNX64/libQAdakDate.so                $DEST_DIR/LNX64
cp -f -a QCommaEdit/LNX64/libQCommaEdit.so              $DEST_DIR/LNX64
cp -f -a QLimitedTextEdit/LNX64/libQLimitedTextEdit.so  $DEST_DIR/LNX64
cp -f -a QSearchEdit/LNX64/libQSearchEdit.so            $DEST_DIR/LNX64
