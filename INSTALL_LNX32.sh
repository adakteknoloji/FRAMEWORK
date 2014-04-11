#! /bin/bash

DEST_DIR=/ADAK/FRAMEWORK


mkdir -p $DEST_DIR

mkdir -p $DEST_DIR/LNX32

cp -f LNX32/libADAK.a  $DEST_DIR/LNX32/

cp -f DESIGNER/LNX32/ADAK_DESIGNER      $DEST_DIR/LNX32/

cp -f UPDATE_DB/LNX32/UPDATE_DB      $DEST_DIR/LNX32/

cp -f -a QAdakDate/LNX32/libQAdakDate.so                $DEST_DIR/LNX32
cp -f -a QCommaEdit/LNX32/libQCommaEdit.so              $DEST_DIR/LNX32
cp -f -a QLimitedTextEdit/LNX32/libQLimitedTextEdit.so  $DEST_DIR/LNX32
cp -f -a QSearchEdit/LNX32/libQSearchEdit.so            $DEST_DIR/LNX32
