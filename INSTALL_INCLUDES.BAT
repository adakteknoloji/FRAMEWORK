@echo off

set DEST_DIR=\ADAK\FRAMEWORK


mkdir %DEST_DIR%
mkdir %DEST_DIR%\INCLUDE
mkdir %DEST_DIR%\TEMPLATE
mkdir %DEST_DIR%\TEMPLATE\YENI_PROJE
mkdir %DEST_DIR%\TEMPLATE\YENI_PROJE\ICONS


copy TEMPLATE\*                         %DEST_DIR%\TEMPLATE
copy TEMPLATE\YENI_PROJE\*              %DEST_DIR%\TEMPLATE\YENI_PROJE
copy TEMPLATE\YENI_PROJE\ICONS\*        %DEST_DIR%\TEMPLATE\YENI_PROJE\ICONS


copy INCLUDE\adak.pri                                %DEST_DIR%\INCLUDE

copy INCLUDE\belge_kernel_defines.h                  %DEST_DIR%\INCLUDE
copy INCLUDE\belge_kernel_class.h                    %DEST_DIR%\INCLUDE
copy INCLUDE\belge_kernel_utils.h                    %DEST_DIR%\INCLUDE
copy INCLUDE\abstract_kernel.h                       %DEST_DIR%\INCLUDE
copy INCLUDE\base_kernel.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\base_event_kernel.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\adak_gruplar.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\base_tree_kernel.h                      %DEST_DIR%\INCLUDE
copy INCLUDE\fis_kernel.h                            %DEST_DIR%\INCLUDE
copy INCLUDE\form_kernel.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\secim_kerneli.h                         %DEST_DIR%\INCLUDE
copy INCLUDE\arama_kernel.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\kernel_utils.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\etiket_kernel.h                         %DEST_DIR%\INCLUDE
copy INCLUDE\etiket_sablonlari.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\adak_defines.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\adak_utils.h                            %DEST_DIR%\INCLUDE
copy INCLUDE\adak_sql.h                              %DEST_DIR%\INCLUDE
copy INCLUDE\adak_sql_struct.h                       %DEST_DIR%\INCLUDE
copy INCLUDE\adak_sql_query.h                        %DEST_DIR%\INCLUDE
copy INCLUDE\adak_sql_enums.h                        %DEST_DIR%\INCLUDE
copy INCLUDE\adak_sql_abstract.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\sql_plugins.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\belge_struct.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\belge_kernel_open.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\menu_kernel.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\sekme_menu_kernel.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\tasarim_utils.h                         %DEST_DIR%\INCLUDE
copy INCLUDE\import_kernel.h                         %DEST_DIR%\INCLUDE
copy INCLUDE\export_kernel.h                         %DEST_DIR%\INCLUDE

copy INCLUDE\adak_std_utils.h                        %DEST_DIR%\INCLUDE

copy INCLUDE\QCommaEdit.h                            %DEST_DIR%\INCLUDE
copy INCLUDE\QSearchEdit.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\QLimitedTextEdit.h                      %DEST_DIR%\INCLUDE
copy INCLUDE\QAdakDate.h                             %DEST_DIR%\INCLUDE
copy INCLUDE\login.h                                 %DEST_DIR%\INCLUDE
copy INCLUDE\login_enum.h                            %DEST_DIR%\INCLUDE
copy INCLUDE\yonetim.h                               %DEST_DIR%\INCLUDE
copy INCLUDE\email.h                                 %DEST_DIR%\INCLUDE
copy INCLUDE\sms.h                                   %DEST_DIR%\INCLUDE
copy INCLUDE\print.h                                 %DEST_DIR%\INCLUDE
copy INCLUDE\report_kernel.h                         %DEST_DIR%\INCLUDE
copy INCLUDE\batch_kernel.h                          %DEST_DIR%\INCLUDE
copy INCLUDE\tree_kernel.h                           %DEST_DIR%\INCLUDE
copy INCLUDE\tree_secim_kernel.h                     %DEST_DIR%\INCLUDE
copy INCLUDE\html_tag.h                              %DEST_DIR%\INCLUDE
copy INCLUDE\adak_framework_plugin.h                 %DEST_DIR%\INCLUDE

