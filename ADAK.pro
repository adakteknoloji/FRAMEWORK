include (INCLUDE/adak.pri)

TARGET     = ADAK
TEMPLATE   = lib
CONFIG    += static

QT        += sql network webkit

# UTILS_OBJECTS
OBJECTS       +=        UTILS/$$OBJDIR/base_sql.$$OBJ_ext \
                        UTILS/$$OBJDIR/adak_array.$$OBJ_ext \
                        UTILS/$$OBJDIR/dongle.$$OBJ_ext \
                        UTILS/$$OBJDIR/virgul_ekle.$$OBJ_ext \
                        UTILS/$$OBJDIR/round.$$OBJ_ext \
			UTILS/$$OBJDIR/adak_program.$$OBJ_ext \
			UTILS/$$OBJDIR/adak_cursor.$$OBJ_ext \
			UTILS/$$OBJDIR/yaziyla_yaz.$$OBJ_ext \
                        UTILS/$$OBJDIR/mac_address.$$OBJ_ext \
                        UTILS/$$OBJDIR/file_io.$$OBJ_ext \
                        UTILS/$$OBJDIR/memory.$$OBJ_ext  \
                        UTILS/$$OBJDIR/module_names.$$OBJ_ext \
                        UTILS/$$OBJDIR/adak_image.$$OBJ_ext \
                        UTILS/$$OBJDIR/temp_file.$$OBJ_ext \
                        UTILS/$$OBJDIR/adak_message.$$OBJ_ext \
                        UTILS/$$OBJDIR/mali_yil.$$OBJ_ext \
                        UTILS/$$OBJDIR/adak_font.$$OBJ_ext


# SQL_OBJECTS
OBJECTS          +=     SQL/$$OBJDIR/adak_sql.$$OBJ_ext \
                        SQL/$$OBJDIR/adak_sql_abstract.$$OBJ_ext \
                        SQL/$$OBJDIR/adak_sql_query.$$OBJ_ext \
                        SQL/$$OBJDIR/adak_sql_error.$$OBJ_ext \
                        SQL/$$OBJDIR/sql_plugins.$$OBJ_ext \
                        SQL/$$OBJDIR/mysql_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/postgressql_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/sqlite_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/mssql_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/oracle_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/odbc_plugin.$$OBJ_ext \
                        SQL/$$OBJDIR/sql_ddl.$$OBJ_ext

# ODS_IO OBJECTS
OBJECTS          +=     ODS_IO/$$OBJDIR/adak_exporter.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/adak_importer.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/export_kernel.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/import_kernel.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/moc_export_kernel.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/moc_import_kernel.$$OBJ_ext \
                        ODS_IO/$$OBJDIR/qrc_ods_io.$$OBJ_ext \

# ADAK_STD_OBJECTS
OBJECTS          +=     ADAK_STD/$$OBJDIR/adak_std.$$OBJ_ext 

# KERNEL_OBJECTS
OBJECTS       +=        QCommaEdit/$$OBJDIR/QCommaEdit.$$OBJ_ext \
                        QCommaEdit/$$OBJDIR/moc_QCommaEdit.$$OBJ_ext \
                        QSearchEdit/$$OBJDIR/QSearchEdit.$$OBJ_ext \
                        QSearchEdit/$$OBJDIR/moc_QSearchEdit.$$OBJ_ext\
                        QLimitedTextEdit/$$OBJDIR/QLimitedTextEdit.$$OBJ_ext \
                        QLimitedTextEdit/$$OBJDIR/moc_QLimitedTextEdit.$$OBJ_ext\
                        QAdakDate/$$OBJDIR/QAdakDate.$$OBJ_ext \
                        QAdakDate/$$OBJDIR/moc_QAdakDate.$$OBJ_ext\
                        KERNEL/$$OBJDIR/fis_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/moc_fis_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/form_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/batch_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/moc_batch_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/moc_form_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/qrc_kernel_icons.$$OBJ_ext       \
                        KERNEL/$$OBJDIR/secim_kerneli.$$OBJ_ext \
                        KERNEL/$$OBJDIR/moc_secim_kerneli.$$OBJ_ext \
                        KERNEL/$$OBJDIR/arama_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/moc_arama_kernel.$$OBJ_ext \
                        KERNEL/$$OBJDIR/kernel_utils.$$OBJ_ext \
			KERNEL/$$OBJDIR/base_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/moc_base_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/base_event_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/moc_base_event_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/abstract_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/kernel_private_utils.$$OBJ_ext \
			MENU_KERNEL/$$OBJDIR/menu_kernel.$$OBJ_ext \
			MENU_KERNEL/$$OBJDIR/moc_menu_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/sekme_menu_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/moc_sekme_menu_kernel.$$OBJ_ext \
			KERNEL/$$OBJDIR/QAdakTabWidget.$$OBJ_ext \
			KERNEL/$$OBJDIR/moc_QAdakTabWidget.$$OBJ_ext


# GRUP_KERNEL_OBJECTS
OBJECTS       +=        GRUP_KERNEL/$$OBJDIR/base_tree_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_base_tree_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/tree_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_tree_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/base_grup_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/tree_secim_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_tree_secim_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/grup_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_grup_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/grup_kernel_grup_fisi.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_grup_kernel_grup_fisi.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/grup_secim_kernel.$$OBJ_ext \
                        GRUP_KERNEL/$$OBJDIR/moc_grup_secim_kernel.$$OBJ_ext \
			GRUP_KERNEL/$$OBJDIR/tree_arama_kernel.$$OBJ_ext \
			GRUP_KERNEL/$$OBJDIR/moc_tree_arama_kernel.$$OBJ_ext \
			GRUP_KERNEL/$$OBJDIR/adak_gruplar.$$OBJ_ext 


# REPORT_KERNEL_OBJECTS
OBJECTS       +=        REPORT_KERNEL/$$OBJDIR/html_tag.$$OBJ_ext \
                        REPORT_KERNEL/$$OBJDIR/print_utils.$$OBJ_ext \
                        REPORT_KERNEL/$$OBJDIR/report_kernel.$$OBJ_ext \
                        REPORT_KERNEL/$$OBJDIR/etiket_kernel.$$OBJ_ext \
                        REPORT_KERNEL/$$OBJDIR/report_shower.$$OBJ_ext \
                        REPORT_KERNEL/$$OBJDIR/moc_report_shower.$$OBJ_ext 

# BELGE_KERNEL OBJECTS
OBJECTS       +=        BELGE_KERNEL/$$OBJDIR/belge_kernel.$$OBJ_ext \
                        BELGE_KERNEL/$$OBJDIR/belge_kernel_utils.$$OBJ_ext \
                        BELGE_KERNEL/$$OBJDIR/tasarim_degiskeni.$$OBJ_ext \
                        BELGE_KERNEL/$$OBJDIR/dot_matrix.$$OBJ_ext \
                        BELGE_KERNEL/$$OBJDIR/dot_matrix_sabitler.$$OBJ_ext



# TASARIMCI OBJECTS
OBJECTS       +=        TASARIMCI/$$OBJDIR/moc_tasarim_batch_base.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/moc_tasarim_belge_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/moc_tasarim_design_widget.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/moc_tasarim_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/moc_tasarim_menu_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/moc_tasarim_yeni_tasarim_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_batch_base.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_belge_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_design_widget.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_menu_batch.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_secimi.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_utils.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_yeni_tasarim_batch.$$OBJ_ext \
			TASARIMCI/$$OBJDIR/tasarim_belge_degiskenleri_secimi.$$OBJ_ext \
			TASARIMCI/$$OBJDIR/qrc_tasarimci_icons.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_export.$$OBJ_ext \
                        TASARIMCI/$$OBJDIR/tasarim_import.$$OBJ_ext 


                        # TASARIMCI/$$OBJDIR/tasarim_dotmatrix_batch.$$OBJ_ext \
                        # TASARIMCI/$$OBJDIR/moc_tasarim_dotmatrix_batch.$$OBJ_ext \

# LOGIN_OBJECTS
OBJECTS        +=       LOGIN/$$OBJDIR/first_login.$$OBJ_ext \
                        LOGIN/$$OBJDIR/login_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/login_utils.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_login_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_programi_guncellestirin_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_yeni_sunucu_tanimlama_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_yardim_hatirlatma_dialog.$$OBJ_ext \
                        LOGIN/$$OBJDIR/program_start.$$OBJ_ext \
                        LOGIN/$$OBJDIR/programi_guncellestirin_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/yeni_sunucu_tanimlama_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/sql_sunucu_secimi.$$OBJ_ext \
                        LOGIN/$$OBJDIR/yardim_hatirlatma_dialog.$$OBJ_ext \
                        LOGIN/$$OBJDIR/adak_sql_error_dialog.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_adak_sql_error_dialog.$$OBJ_ext \
                        LOGIN/$$OBJDIR/veritabani_secimi.$$OBJ_ext \
                        LOGIN/$$OBJDIR/yonetici_izni_batch.$$OBJ_ext \
                        LOGIN/$$OBJDIR/moc_yonetici_izni_batch.$$OBJ_ext

OBJECTS        +=       YONETIM/$$OBJDIR/bilgisayarlar_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/bilgisayar_secimi.$$OBJ_ext \
                        YONETIM/$$OBJDIR/hata_istek_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/kullanici_degistir_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/kullanicilar_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/kullanici_secimi.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_bilgisayarlar_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_hata_istek_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_kullanici_degistir_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_kullanicilar_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_posta_sunucusu_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_sifre_degistirme_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_sms_sunucusu_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_veritabanlari_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_yonetim_007_arama.$$OBJ_ext \
                        YONETIM/$$OBJDIR/moc_yonetim_hakkinda.$$OBJ_ext \
                        YONETIM/$$OBJDIR/posta_sunucusu_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/program_secimi.$$OBJ_ext \
                        YONETIM/$$OBJDIR/qrc_yonetim_icons.$$OBJ_ext \
                        YONETIM/$$OBJDIR/sifre_degistirme_batch.$$OBJ_ext \
                        YONETIM/$$OBJDIR/sms_sunucusu_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/veritabanlari_formu.$$OBJ_ext \
                        YONETIM/$$OBJDIR/yonetim_007_arama.$$OBJ_ext \
                        YONETIM/$$OBJDIR/yonetim_hakkinda.$$OBJ_ext \
                        YONETIM/$$OBJDIR/yonetim_utils.$$OBJ_ext

                        
# EMAIL_OBJECTS
OBJECTS       +=        EMAIL/$$OBJDIR/email_sender.$$OBJ_ext \
                        EMAIL/$$OBJDIR/moc_email_sender.$$OBJ_ext \
                        EMAIL/$$OBJDIR/email_sender_gui.$$OBJ_ext \
                        EMAIL/$$OBJDIR/moc_email_sender_gui.$$OBJ_ext

# SMS_OBJECTS
OBJECTS       +=        SMS/$$OBJDIR/sms_sender.$$OBJ_ext \
                        SMS/$$OBJDIR/moc_sms_sender.$$OBJ_ext \
                        SMS/$$OBJDIR/sms_sender_gui.$$OBJ_ext \
                        SMS/$$OBJDIR/moc_sms_sender_gui.$$OBJ_ext \
                        SMS/$$OBJDIR/sms_server_abstract.$$OBJ_ext \
                        SMS/$$OBJDIR/sms_servers.$$OBJ_ext \
                        SMS/$$OBJDIR/clickatell.$$OBJ_ext 
