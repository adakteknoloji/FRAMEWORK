#! /bin/bash

DEST_DIR=../E9/ADAK


mkdir -p $DEST_DIR

mkdir -p $DEST_DIR/INCLUDE
mkdir -p $DEST_DIR/TEMPLATE

cp -r -f TEMPLATE/*                                   $DEST_DIR/TEMPLATE

cp -f INCLUDE/adak.pri                                $DEST_DIR/INCLUDE

cp -f INCLUDE/belge_kernel_defines.h                  $DEST_DIR/INCLUDE
cp -f INCLUDE/belge_kernel_class.h                    $DEST_DIR/INCLUDE
cp -f INCLUDE/belge_kernel_utils.h                    $DEST_DIR/INCLUDE
cp -f INCLUDE/abstract_kernel.h                       $DEST_DIR/INCLUDE
cp -f INCLUDE/base_kernel.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/base_event_kernel.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_gruplar.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/base_tree_kernel.h                      $DEST_DIR/INCLUDE
cp -f INCLUDE/fis_kernel.h                            $DEST_DIR/INCLUDE
cp -f INCLUDE/form_kernel.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/secim_kerneli.h                         $DEST_DIR/INCLUDE
cp -f INCLUDE/arama_kernel.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/kernel_utils.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/etiket_kernel.h                         $DEST_DIR/INCLUDE
cp -f INCLUDE/etiket_sablonlari.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_defines.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_utils.h                            $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_sql.h                              $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_sql_struct.h                       $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_sql_query.h                        $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_sql_enums.h                        $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_sql_abstract.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/sql_plugins.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/belge_struct.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/belge_kernel_open.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/menu_kernel.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/sekme_menu_kernel.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/tasarim_utils.h                         $DEST_DIR/INCLUDE
cp -f INCLUDE/import_kernel.h                         $DEST_DIR/INCLUDE
cp -f INCLUDE/export_kernel.h                         $DEST_DIR/INCLUDE

cp -f INCLUDE/adak_std_utils.h                        $DEST_DIR/INCLUDE

cp -f INCLUDE/QCommaEdit.h                            $DEST_DIR/INCLUDE
cp -f INCLUDE/QSearchEdit.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/QLimitedTextEdit.h                      $DEST_DIR/INCLUDE
cp -f INCLUDE/QAdakDate.h                             $DEST_DIR/INCLUDE
cp -f INCLUDE/login.h                                 $DEST_DIR/INCLUDE
cp -f INCLUDE/login_enum.h                            $DEST_DIR/INCLUDE
cp -f INCLUDE/yonetim.h                               $DEST_DIR/INCLUDE
cp -f INCLUDE/email.h                                 $DEST_DIR/INCLUDE
cp -f INCLUDE/sms.h                                   $DEST_DIR/INCLUDE
cp -f INCLUDE/print.h                                 $DEST_DIR/INCLUDE
cp -f INCLUDE/report_kernel.h                         $DEST_DIR/INCLUDE
cp -f INCLUDE/batch_kernel.h                          $DEST_DIR/INCLUDE
cp -f INCLUDE/tree_kernel.h                           $DEST_DIR/INCLUDE
cp -f INCLUDE/tree_secim_kernel.h                     $DEST_DIR/INCLUDE
cp -f INCLUDE/html_tag.h                              $DEST_DIR/INCLUDE
cp -f INCLUDE/adak_framework_plugin.h                 $DEST_DIR/INCLUDE

