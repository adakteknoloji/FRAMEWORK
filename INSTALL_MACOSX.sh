#! /bin/bash

DEST_DIR=../E9/ADAK


mkdir -p $DEST_DIR

mkdir -p $DEST_DIR/MACOSX

cp -f MACOSX/libADAK.a                      $DEST_DIR/MACOSX/

cp -f -r DESIGNER/MACOSX/ADAK_DESIGNER.app  $DEST_DIR/MACOSX/

cp -f -r UPDATE_DB/MACOSX/UPDATE_DB.app     $DEST_DIR/MACOSX/

cp -f -a QAdakDate/MACOSX/libQAdakDate.dylib                $DEST_DIR/MACOSX
cp -f -a QCommaEdit/MACOSX/libQCommaEdit.dylib              $DEST_DIR/MACOSX
cp -f -a QLimitedTextEdit/MACOSX/libQLimitedTextEdit.dylib  $DEST_DIR/MACOSX
cp -f -a QSearchEdit/MACOSX/libQSearchEdit.dylib            $DEST_DIR/MACOSX
