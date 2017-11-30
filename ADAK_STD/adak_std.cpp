#include <QtWidgets/QComboBox>

#include "sql_ddl.h"
#include "adak_std.h"
#include "ulkeler.h"
#include "iller.h"
#include "cinsiyetler.h"
#include "ogrenim_durumlari.h"
#include "gsm_kodlari.h"
#include "dovizler.h"
#include "ilceler.h"
#include "vergi_daireleri.h"
#include "bankalar.h"
#include "banka_subeleri.h"
#include "medeni_haller.h"
#include "kan_gruplari.h"
#include "sektorler.h"
#include "unvanlar.h"
#include "meslekler.h"

extern ADAK_SQL * G_YONETIM_DB;

/******************************************************************/
/*                        ADD_ADAK_STD                           */
/******************************************************************/

void ADD_ADAK_STD()
{
    ADAK_STD * adak_std = new ADAK_STD();

    G_YONETIM_DB->START_TRANSACTION();

    adak_std->DOVIZLERI_EKLE();
    adak_std->ULKELERI_EKLE();
    adak_std->ILLERI_EKLE();
    adak_std->ILCELERI_EKLE();
    adak_std->CINSIYETLERI_EKLE();
    adak_std->OGRENIM_DURUMLARINI_EKLE();
    adak_std->GSM_KODLARINI_EKLE();
    adak_std->VERGI_DAIRELERINI_EKLE();
    adak_std->BANKALARI_EKLE();
    adak_std->BANKA_SUBELERINI_EKLE();
    adak_std->KAN_GRUPLARINI_EKLE();
    adak_std->MEDENI_HALLERI_EKLE();
    adak_std->SEKTORLERI_EKLE();
    adak_std->MESLEKLERI_EKLE();
    adak_std->UNVANLARI_EKLE();

    G_YONETIM_DB->COMMIT_TRANSACTION();
}

/******************************************************************/
/*                   ADAK_STD::ADAK_STD                         */
/******************************************************************/

ADAK_STD::ADAK_STD()
{

}

/******************************************************************/
/*                   ADAK_STD::DOVIZLERI_EKLE                    */
/******************************************************************/

bool ADAK_STD::DOVIZLERI_EKLE()
{
    SQL_DDL   ddl_query (G_YONETIM_DB);
    SQL_QUERY query     (G_YONETIM_DB);

    QString doviz_string = "";
    int i = 0;

    query.PREPARE_SELECT("adak_dovizler","doviz_kodu","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            doviz_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_dovizler","doviz_id","doviz_kodu,doviz_adi");

    while (strcmp(dovizler[i][0],"")) {
        if (doviz_string.contains(dovizler[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE(QString("'"+QString::fromUtf8(dovizler[i][0])+"','"+QString::fromUtf8(dovizler[i][1])+"'"));
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();


    return true;
}

/******************************************************************/
/*                   ADAK_STD::ULKELERI_EKLE                     */
/******************************************************************/

bool ADAK_STD::ULKELERI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString ulkeler_string = "";
    QStringList doviz_kodlari;
    QList<int>  doviz_idleri;

    query.PREPARE_SELECT("adak_ulkeler","ulke_telefon_kodu");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            ulkeler_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    query.PREPARE_SELECT("adak_dovizler","doviz_id,doviz_kodu","","");
    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            doviz_kodlari << query.VALUE(1).toString();
            doviz_idleri  << query.VALUE(0).toInt();
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_ulkeler","ulke_id","doviz_id,ulke_telefon_kodu,ulke_plaka_kodu,ulke_adi");

    int i = 0;

    QString temp_doviz_kodu = NULL;
    int doviz_id = -1;

    while (strcmp(ulkeler[i][0],"")) {
        if (ulkeler_string.contains(ulkeler[i][1]) EQ true) {
            i++;
            continue;
        }

        if(temp_doviz_kodu NE NULL) {
            if (temp_doviz_kodu NE QString(ulkeler[i][3])) {
                doviz_id = -1;
            }
        }

        if (doviz_id EQ -1) {
            for (int j=0;j<doviz_kodlari.size();j++) {
                if (doviz_kodlari.at(j) EQ QString(ulkeler[i][3])) {
                    doviz_id        = doviz_idleri.at(j);
                    temp_doviz_kodu = doviz_kodlari.at(j);
                    break;
                }
            }
        }

        QString values = "'" + QVariant(doviz_id).toString() + "','" + QString::fromUtf8(ulkeler[i][1]) + "','" +
                         QString::fromUtf8(ulkeler[i][2]) + "','" + QString::fromUtf8(ulkeler[i][0]) + "'";

        ddl_query.ADD_VALUES_TO_QUEUE(values);
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::ILLERI_EKLE                       */
/******************************************************************/

bool ADAK_STD::ILLERI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_QUERY sql_query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString il_string = "";
    QStringList ulke_adlari;
    QList<int>  ulke_idleri;

    sql_query.PREPARE_SELECT("adak_iller","plaka_kodu","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            il_string.append(QString("%1 ").arg(sql_query.VALUE(0).toString()));
        }
    }

    sql_query.PREPARE_SELECT("adak_ulkeler","ulke_id,ulke_adi","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            ulke_adlari << sql_query.VALUE(1).toString();
            ulke_idleri << sql_query.VALUE(0).toInt();
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_iller","il_id","ulke_id,telefon_kodu1,telefon_kodu2,plaka_kodu,il_adi");

    int i = 0;

    QString temp_ulke_adi = NULL;
    int ulke_id = -1;

    while (strcmp(iller[i][0],"")) {
        if (il_string.contains(iller[i][4]) EQ true) {
            i++;
            continue;
        }

        if(temp_ulke_adi NE NULL) {
            if (temp_ulke_adi NE QString(iller[i][0])) {
                ulke_id = -1;
            }
        }

        if (ulke_id EQ -1) {
            for (int j=0;j<ulke_adlari.size();j++) {
                if (ulke_adlari.at(j) EQ QString(iller[i][0])) {
                    ulke_id        = ulke_idleri.at(j);
                    temp_ulke_adi  = ulke_adlari.at(j);
                    break;
                }
            }
        }

        QString columns = "'" + QVariant(ulke_id).toString() + "','" + QString::fromUtf8(iller[i][2]) + "','" + QString::fromUtf8(iller[i][3]) + "','" +
                          QString::fromUtf8(iller[i][4]) + "','" + QString::fromUtf8(iller[i][1]) + "'";


        ddl_query.ADD_VALUES_TO_QUEUE(columns);
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::ILCELERI_EKLE                     */
/******************************************************************/

bool ADAK_STD::ILCELERI_EKLE()
{
    SQL_QUERY sql_query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    int i = 0;
    QString ilceler_string = "";

    sql_query.PREPARE_SELECT("adak_ilceler","ilce_adi","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            ilceler_string.append(QString("%1 ").arg(sql_query.VALUE(0).toString()));
        }
    }

    sql_query.PREPARE_SELECT("adak_iller","il_id,il_adi","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            il_adlari << sql_query.VALUE(1).toString();
            il_idleri << sql_query.VALUE(0).toInt();
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_ilceler","ilce_id","il_id, ilce_adi");

    QString temp_il_adi = NULL;
    int il_id = -1;

    while (strcmp(ilceler[i][0],"")) {
        if (ilceler_string.contains(ilceler[i][1]) EQ true) {
            i++;
            continue;
        }

        if(temp_il_adi NE NULL) {
            if (temp_il_adi NE QString(ilceler[i][0])) {
                il_id = -1;
            }
        }

        if (il_id EQ -1) {
            for (int j=0;j<il_adlari.size();j++) {
                if (il_adlari.at(j) EQ QString(ilceler[i][0])) {
                    il_id         = il_idleri.at(j);
                    temp_il_adi   = il_adlari.at(j);
                    break;
                }
            }
        }

        ddl_query.ADD_VALUES_TO_QUEUE(QVariant(il_id).toString()+",'"+QString::fromUtf8(ilceler[i][1])+"'");
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::CINSIYETLERI_EKLE                 */
/******************************************************************/

bool ADAK_STD::CINSIYETLERI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);
    int i = 0;
    QString cinsiyetler_string = "";

    query.PREPARE_SELECT("adak_cinsiyetler","cinsiyet_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            cinsiyetler_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_cinsiyetler","cinsiyet_id","cinsiyet_adi");

    while (strcmp(cinsiyetler[i][0],"")) {
        if (cinsiyetler_string.contains(cinsiyetler[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'" + QString::fromUtf8(cinsiyetler[i][0]) + "'");

        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::OGRENIM_DURUMLARINI_EKLE          */
/******************************************************************/

bool ADAK_STD::OGRENIM_DURUMLARINI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    int i = 0;
    QString ogrenim_durumlari_string = "";

    query.PREPARE_SELECT("adak_ogrenim_durumlari","ogrenim_durumu_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            ogrenim_durumlari_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_ogrenim_durumlari","ogrenim_durumu_id","ogrenim_durumu_adi");

    while (strcmp(ogrenim_durumlari[i][0],"")) {
        if (ogrenim_durumlari_string.contains(ogrenim_durumlari[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'" + QString::fromUtf8(ogrenim_durumlari[i][0]) + "'");

        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::GSM_KODLARINI_EKLE                */
/******************************************************************/

bool ADAK_STD::GSM_KODLARINI_EKLE()
{
    SQL_QUERY sql_query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    int i = 0;
    QString gsm_kodlari_string = "";
    QStringList ulke_adlari;
    QList<int>  ulke_idleri;

    sql_query.PREPARE_SELECT("adak_gsm_kodlari","gsm_kodu","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            gsm_kodlari_string.append(QString("%1 ").arg(sql_query.VALUE(0).toString()));
        }
    }

    sql_query.PREPARE_SELECT("adak_ulkeler","ulke_id,ulke_adi","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            ulke_adlari << sql_query.VALUE(1).toString();
            ulke_idleri << sql_query.VALUE(0).toInt();
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_gsm_kodlari","gsm_id","ulke_id, gsm_kodu,gsm_adi");

    QString temp_ulke_adi = NULL;
    int ulke_id = -1;

    while (strcmp(gsm_kodlari[i][0],"")) {
        if (gsm_kodlari_string.contains(gsm_kodlari[i][2]) EQ true) {
            i++;
            continue;
        }

        if(temp_ulke_adi NE NULL) {
            if (temp_ulke_adi NE QString(iller[i][0])) {
                ulke_id = -1;
            }
        }

        if (ulke_id EQ -1) {
            for (int j=0;j<ulke_adlari.size();j++) {
                if (ulke_adlari.at(j) EQ QString(gsm_kodlari[i][0])) {
                    ulke_id         = ulke_idleri.at(j);
                    temp_ulke_adi   = ulke_adlari.at(j);
                    break;
                }
            }
        }

        ddl_query.ADD_VALUES_TO_QUEUE(QVariant(ulke_id).toString()+",'"+QString::fromUtf8(gsm_kodlari[i][2])+"','"+QString::fromUtf8(gsm_kodlari[i][1])+"'");

        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::VERGI_DAIRELERINI_EKLE            */
/******************************************************************/

bool ADAK_STD::VERGI_DAIRELERINI_EKLE()
{
    SQL_QUERY sql_query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);
    int i = 0;
    QString vergi_daireleri_string = "";


    sql_query.PREPARE_SELECT("adak_vergi_daireleri","vergi_dairesi_adi","","");

    if ( sql_query.SELECT() NE 0 ) {
        for (int i=0;i < sql_query.NUM_OF_ROWS();i++ ) {
            sql_query.NEXT();
            vergi_daireleri_string.append(QString("%1 ").arg(sql_query.VALUE(0).toString()));
        }
    }

    QString temp_il_adi = NULL;

    ddl_query.START_MULTIPLE_INSERT("adak_vergi_daireleri","vergi_dairesi_id","il_id, vergi_dairesi_adi");

    int il_id = -1;

    while (strcmp(vergi_daireleri[i][0],"")) {
        if (vergi_daireleri_string.contains(vergi_daireleri[i][1]) EQ true) {
            i++;
            continue;
        }

        if(temp_il_adi NE NULL) {
            if (temp_il_adi NE QString(vergi_daireleri[i][0])) {
                il_id = -1;
            }
        }

        if (il_id EQ -1) {
            for (int j=0;j<il_adlari.size();j++) {
                if (il_adlari.at(j) EQ QString(vergi_daireleri[i][0])) {
                    il_id         = il_idleri.at(j);
                    temp_il_adi   = il_adlari.at(j);
                    break;
                }
            }
        }

        ddl_query.ADD_VALUES_TO_QUEUE(QVariant(il_id).toString() + ",'" + QString::fromUtf8(vergi_daireleri[i][1])+"'");

        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::BANKALARI_EKLE                    */
/******************************************************************/

bool ADAK_STD::BANKALARI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);
    QString banka_string = "";

    query.PREPARE_SELECT("adak_bankalar","banka_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            banka_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_bankalar","banka_id","banka_adi,banka_adresi,banka_telefonu,banka_web_adresi,banka_email");

    int i = 0;

    while (strcmp(bankalar[i][0],"")) {
        if (banka_string.contains(bankalar[i][0]) EQ true) {
            i++;
            continue;
        }

        QString columns = "'" + QString::fromUtf8(bankalar[i][0]) + "','" + QString::fromUtf8(bankalar[i][1]) + "','" + QString::fromUtf8(bankalar[i][2]) +
                          "','" + QString::fromUtf8(bankalar[i][3]) + "','" + QString::fromUtf8(bankalar[i][4]) + "'";


        ddl_query.ADD_VALUES_TO_QUEUE(columns);
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::BANKA_SUBELERINI_EKLE             */
/******************************************************************/

bool ADAK_STD::BANKA_SUBELERINI_EKLE()
{
    SQL_QUERY query    (G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);
    QString banka_subeleri_string = "";
    QList<int> banka_idleri;
    QStringList banka_adlari;
    QList<int> ilce_idleri;
    QList<int> ilce_adlari;
    QList<int> ilce_il_idleri;

    query.PREPARE_SELECT("adak_banka_subeleri","sube_web_adresi,sube_kodu","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            banka_subeleri_string.append(QString("%1%2 ").arg(query.VALUE(0).toString()).arg(query.VALUE(1).toString()));
        }
    }

    query.PREPARE_SELECT("adak_bankalar","banka_id,banka_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            banka_idleri << query.VALUE(0).toInt();
            banka_adlari << query.VALUE(1).toString();
        }
    }

    query.PREPARE_SELECT("adak_ilceler","ilce_id,il_id,ilce_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            ilce_idleri    << query.VALUE(0).toInt();
            ilce_il_idleri << query.VALUE(1).toInt();
            ilce_adlari    << query.VALUE(2).toInt();
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_banka_subeleri","banka_sube_id","banka_id, il_id, ilce_id, sube_kodu,"
                                                         "sube_adi, sube_telefonu, sube_adresi, sube_web_adresi, sube_email");
    int index = 0;

    while (strcmp(banka_subeleri[index][0],"")) {
        int banka_id = -1;

        for (int i=0;i<banka_adlari.size();i++) {
            if (banka_adlari.at(i) EQ QString(banka_subeleri[index][0])) {
                banka_id = banka_idleri.at(i);
                break;
            }
        }

        if ( banka_id EQ -1 ) {
            index++;
            continue;
            //return true;
        }

        int il_id = -1;
        for (int i=0;i<il_adlari.size();i++) {
            if (il_adlari.at(i) EQ QString(banka_subeleri[index][1])) {
                il_id = il_idleri.at(i);
                break;
            }
        }

        if ( il_id EQ -1 ) {
            index++;
            continue;
            //return true;
        }

        int ilce_id = 0;


        QString banka_unique_name = QString("%1%2").arg(banka_subeleri[index][7]).arg(banka_subeleri[index][4]);

        if (banka_subeleri_string.contains(banka_unique_name) EQ true ) {
            index++;
            continue;
        }

        QString columns = QVariant(banka_id).toString() + "," + QVariant(il_id).toString() + "," + QVariant(ilce_id).toString() + ",'" +
                          QString::fromUtf8(banka_subeleri[index][4]) + "','" + QString::fromUtf8(banka_subeleri[index][3]) + "','" +
                          QString::fromUtf8(banka_subeleri[index][5]) + "','" + QString::fromUtf8(banka_subeleri[index][6]) + "','" +
                          QString::fromUtf8(banka_subeleri[index][7]) + "','" + QString::fromUtf8(banka_subeleri[index][8]) + "'";

        ddl_query.ADD_VALUES_TO_QUEUE(columns);

        index++;

    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::MEDENI_HALLERI_EKLE               */
/******************************************************************/

bool ADAK_STD::MEDENI_HALLERI_EKLE()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString medeni_haller_string = "";

    query.PREPARE_SELECT("adak_medeni_haller","medeni_hal_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            medeni_haller_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_medeni_haller","medeni_hal_id","medeni_hal_adi");

    int i = 0;

    while (strcmp(medeni_haller[i][0],"")) {
        if (medeni_haller_string.contains(medeni_haller[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'"+QString::fromUtf8(medeni_haller[i][0]) + "'");
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::KAN_GRUPLARINI_EKLE               */
/******************************************************************/

bool ADAK_STD::KAN_GRUPLARINI_EKLE()
{
    SQL_QUERY query    (G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString kan_gruplari_string = "";

    query.PREPARE_SELECT("adak_kan_gruplari","kan_grubu_adi","","");

    if ( query.SELECT() NE 0 ) {
        for (int i=0;i < query.NUM_OF_ROWS();i++ ) {
            query.NEXT();
            kan_gruplari_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_kan_gruplari","kan_grubu_id","kan_grubu_adi");

    int i = 0;

    while (strcmp(kan_gruplari[i][0],"")) {
        if (kan_gruplari_string.contains(kan_gruplari[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'"+QString::fromUtf8(kan_gruplari[i][0])+"'");
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************/
/*                   ADAK_STD::SEKTORLERI_EKLE                   */
/******************************************************************/

bool ADAK_STD::SEKTORLERI_EKLE ()
{
    SQL_QUERY query    (G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString sektorler_string = "";

    query.PREPARE_SELECT("adak_sektorler","sektor_adi","","");

    if ( query.SELECT() NE 0 ) {
        for ( int i = 0; i < query.NUM_OF_ROWS(); i++ ) {
            query.NEXT();
            sektorler_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_sektorler","sektor_id","sektor_adi");

    int i = 0;

    while (strcmp(sektorler[i][0],"")) {
        if (sektorler_string.contains(sektorler[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'"+QString::fromUtf8(sektorler[i][0])+"'");
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}


/******************************************************************
                     ADAK_STD::MESLEKLERI_EKLE
******************************************************************/

bool ADAK_STD::MESLEKLERI_EKLE()
{
    SQL_QUERY query    (G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString meslekler_string = "";

    query.PREPARE_SELECT("adak_meslekler","meslek_adi","","");

    if ( query.SELECT() NE 0 ) {
        for ( int i = 0; i < query.NUM_OF_ROWS(); i++ ) {
            query.NEXT();
            meslekler_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_meslekler","meslek_id","meslek_adi");

    int i = 0;

    while (strcmp(meslekler[i][0],"")) {
        if (meslekler_string.contains(meslekler[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'"+QString::fromUtf8(meslekler[i][0])+"'");
        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

/******************************************************************
                     ADAK_STD::UNVANLARI_EKLE
******************************************************************/

bool ADAK_STD::UNVANLARI_EKLE ()
{
    SQL_QUERY query(G_YONETIM_DB);
    SQL_DDL   ddl_query(G_YONETIM_DB);

    QString unvanlar_string = "";

    query.PREPARE_SELECT("adak_unvanlar","unvan_adi","","");

    if ( query.SELECT() NE 0 ) {
        for ( int i = 0; i < query.NUM_OF_ROWS(); i++ ) {
            query.NEXT();
            unvanlar_string.append(QString("%1 ").arg(query.VALUE(0).toString()));
        }
    }

    ddl_query.START_MULTIPLE_INSERT("adak_unvanlar","unvan_id","unvan_adi");

    int i = 0;

    while (strcmp(unvanlar[i][0],"")) {
        if (unvanlar_string.contains(unvanlar[i][0]) EQ true) {
            i++;
            continue;
        }

        ddl_query.ADD_VALUES_TO_QUEUE("'"+QString::fromUtf8(unvanlar[i][0])+"'");

        i++;
    }

    ddl_query.COMMIT_MULTIPLE_INSERT();

    return true;
}

///////////////////////////////////////////
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// GET_FUNCTIONS begin here
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////






/**************************************************/
/*                GET_ULKELER                     */
/**************************************************/

QStringList GET_ULKELER( QComboBox * comboBox )
{
    QStringList values;

    int i = 0;

    while (strcmp(ulkeler[i][0],"")) {
            values <<  QString::fromUtf8(ulkeler[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                GET_ULKELER                     */
/**************************************************/

QString GET_ULKE_TELEFON_KODU(QString ulke_adi)
{
    int i = 0;

    while (strcmp(ulkeler[i][0],"")) {
        if (strcmp(ulkeler[i][0],ulke_adi.toUtf8().data()) NE 0) {
            i++;
            continue;
        }

        return ulkeler[i][1];
    }

    return "";
}

/**************************************************/
/*                GET_UYRUKLAR                    */
/**************************************************/

QStringList GET_UYRUKLAR( QComboBox * comboBox )
{
    return GET_ULKELER(comboBox);
}

/**************************************************/
/*          GET_VERGI_DAIRELERI                   */
/**************************************************/

QStringList GET_VERGI_DAIRELERI(QComboBox * comboBox,QString il_adi)
{
    QStringList values;

    int i = 0;

    while (strcmp(vergi_daireleri[i][1],"")) {
        if (il_adi.isEmpty() EQ false) {
            if (strcmp(vergi_daireleri[i][0],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(vergi_daireleri[i][1]);
        i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;

}

/**************************************************/
/*                GET_DOVIZLER                    */
/**************************************************/

QStringList GET_DOVIZLER(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(dovizler[i][1],"")) {
            values <<  QString::fromUtf8(dovizler[i][1]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                 GET_ILLER                      */
/**************************************************/

QString GET_IL_ALAN_KODU (QString il_adi , QString ulke_adi)
{
    int i = 0;

    while (strcmp(iller[i][1],"")) {
        if (ulke_adi.isEmpty() EQ false) {
            if (strcmp(iller[i][0],ulke_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (il_adi.isEmpty() EQ false) {
            if (strcmp(iller[i][1],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }


        return iller[i][2];
    }

    return "";
}

/**************************************************/
/*                 GET_ILLER                      */
/**************************************************/

QStringList GET_ILLER(QComboBox * comboBox,QString ulke_adi)
{
    QStringList values;

    int i = 0;

    while (strcmp(iller[i][1],"")) {
        if (ulke_adi.isEmpty() EQ false) {
            if (strcmp(iller[i][0],ulke_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(iller[i][1]);
        i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                 GET_ILCELER                    */
/**************************************************/

QStringList GET_ILCELER(QComboBox * comboBox,QString il_adi)
{
    QStringList values;

    int i = 0;

    while (strcmp(ilceler[i][1],"")) {
        if (il_adi.isEmpty() EQ false) {
            if (strcmp(ilceler[i][0],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(ilceler[i][1]);
        i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                GET_CINSIYETLER                 */
/**************************************************/

QStringList GET_CINSIYETLER(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(cinsiyetler[i][0],"")) {
            values <<  QString::fromUtf8(cinsiyetler[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*             GET_OGRENIM_DURUMLARI              */
/**************************************************/

QStringList GET_OGRENIM_DURUMLARI(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(ogrenim_durumlari[i][0],"")) {
            values <<  QString::fromUtf8(ogrenim_durumlari[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*               GET_GSM_KODLARI                  */
/**************************************************/

QStringList GET_ULKE_TELEFON_KODLARI(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (  ( strcmp(ulkeler[i][0], "" ) NE 0 ) ) {
        if ( QString::fromUtf8(ulkeler[i][1]).isEmpty() EQ false ) {
            values <<  QString::fromUtf8(ulkeler[i][1]);
        }
        i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*               GET_GSM_KODLARI                  */
/**************************************************/

QStringList GET_GSM_KODLARI(QComboBox * comboBox,QString ulke_telefon_kodu ,QString ulke_adi)
{
    int i = 0;

    if (ulke_telefon_kodu.isEmpty() EQ false) {
        while (  ( strcmp(ulkeler[i][0], "" ) NE 0 ) ) {
            if ( strcmp ( ulkeler[i][1], ulke_telefon_kodu.toUtf8().data() ) EQ 0 ) {
                ulke_adi = ulkeler[i][0];
                break;
            }
            i++;
        }
    }

    QStringList values;

    i = 0;

    while (strcmp(gsm_kodlari[i][2],"")) {
        if (ulke_adi.isEmpty() EQ false) {
            if (strcmp(gsm_kodlari[i][0],ulke_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(gsm_kodlari[i][2]);
        i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*               GET_ALAN_KODLARI                 */
/**************************************************/

QStringList GET_ALAN_KODLARI ( QComboBox * comboBox, QString ulke_telefon_kodu, QString ulke_adi )
{
    int i = 0;

    if (ulke_telefon_kodu.isEmpty() EQ false) {
        while (  ( strcmp(ulkeler[i][0], "" ) NE 0 ) ) {
            if ( strcmp ( ulkeler[i][1], ulke_telefon_kodu.toUtf8().data() ) EQ 0 ) {
                ulke_adi = ulkeler[i][0];
                break;
            }
            i++;
        }
    }

    if (ulke_adi.isEmpty() EQ true) {
        return QStringList();
    }

    QStringList values;

    i = 0;

    while (  ( strcmp(iller[i][2], "" ) NE 0 ) ) {
        if ( ulke_adi.isEmpty() EQ false ) {
            if ( strcmp ( iller[i][0], ulke_adi.toUtf8().data() ) NE 0 ) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(iller[i][2]);
        if ( QString::fromUtf8(iller[i][3] ).isEmpty() EQ false ) {
            values << QString::fromUtf8(iller[i][3]);
        }
        i++;
    }

    values.sort();

    i = 0;

    while (strcmp(gsm_kodlari[i][2],"")) {
        if (ulke_adi.isEmpty() EQ false) {
            if (strcmp(gsm_kodlari[i][0],ulke_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(gsm_kodlari[i][2]);
        i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                GET_BANKALAR                    */
/**************************************************/

QStringList GET_BANKALAR(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(bankalar[i][3],"")) {
            values <<  QString::fromUtf8(bankalar[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*              GET_BANKA_SUBELERI                */
/**************************************************/

QStringList GET_BANKA_SUBE_ADLARI(QComboBox * comboBox,QString banka_adi,QString il_adi)
{

    QStringList values;

    int i = 0;

    while (strcmp(banka_subeleri[i][3],"")) {
        if (banka_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][0],banka_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (il_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][1],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(banka_subeleri[i][3]);
        i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*              GET_BANKA_SUBE_KODLARI            */
/**************************************************/

QStringList GET_BANKA_SUBE_KODLARI(QComboBox * comboBox,QString banka_adi,QString il_adi)
{

    QStringList values;

    int i = 0;

    while (strcmp(banka_subeleri[i][3],"")) {
        if (banka_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][0],banka_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (il_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][1],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        values <<  QString::fromUtf8(banka_subeleri[i][4]);
        i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*              GET_BANKA_SUBE_ADI                */
/**************************************************/

QString GET_BANKA_SUBE_ADI ( QString banka_adi , QString sube_kodu )
{
    int i = 0;

    while (strcmp(banka_subeleri[i][0],"")) {
        if (banka_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][0],banka_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (sube_kodu.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][4],sube_kodu.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        return QString::fromUtf8(banka_subeleri[i][3]);
    }

    return "";
}

/**************************************************/
/*              GET_BANKA_SUBE_KODU               */
/**************************************************/

QString GET_BANKA_SUBE_KODU ( QString banka_adi , QString sube_adi , QString il_adi )
{
    int i = 0;

    while (strcmp(banka_subeleri[i][0],"")) {
        if (banka_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][0],banka_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (sube_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][3],sube_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        if (il_adi.isEmpty() EQ false) {
            if (strcmp(banka_subeleri[i][1],il_adi.toUtf8().data()) NE 0) {
                i++;
                continue;
            }
        }

        return QString::fromUtf8(banka_subeleri[i][4]);
    }

    return "";
}

/**************************************************/
/*              GET_KAN_GRUPLARI                  */
/**************************************************/

QStringList GET_KAN_GRUPLARI(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(kan_gruplari[i][0],"")) {
            values <<  QString::fromUtf8(kan_gruplari[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*              GET_MEDENI_HALLER                 */
/**************************************************/

QStringList GET_MEDENI_HALLER(QComboBox * comboBox)
{
    QStringList values;

    int i = 0;

    while (strcmp(medeni_haller[i][0],"")) {
            values <<  QString::fromUtf8(medeni_haller[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                GET_SEKTORLER                   */
/**************************************************/

QStringList GET_SEKTORLER ( QComboBox * comboBox )
{
    QStringList values;

    int i = 0;

    while (strcmp(sektorler[i][0],"")) {
            values <<  QString::fromUtf8(sektorler[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}


/**************************************************/
/*                GET_MESLEKLER                   */
/**************************************************/

QStringList GET_MESLEKLER ( QComboBox * comboBox )
{
    QStringList values;

    int i = 0;

    while (strcmp(meslekler[i][0],"")) {
            values <<  QString::fromUtf8(meslekler[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/**************************************************/
/*                GET_UNVANLAR                    */
/**************************************************/

QStringList GET_UNVANLAR ( QComboBox * comboBox )
{
    QStringList values;

    int i = 0;

    while (strcmp(unvanlar[i][0],"")) {
            values <<  QString::fromUtf8(unvanlar[i][0]);
            i++;
    }

    if ( comboBox NE NULL ) {
        values.sort();
        comboBox->clear();
        comboBox->addItems(values);
        comboBox->setCurrentIndex (-1);
        comboBox->setEditable(true);
        return QStringList();
    }

    return values;
}

/************************************************************/
/*               FIND_AND_SET_COMBOBOX_TEXT                 */
/************************************************************/

void FIND_AND_SET_COMBOBOX_TEXT ( QComboBox * p_comboBox, QString p_text )
{
    int comboBox_text_index = p_comboBox->findText ( p_text );

    if ( comboBox_text_index EQ -1 ) {
        p_comboBox->addItem (p_text);
        comboBox_text_index = p_comboBox->findText ( p_text );
    }

    p_comboBox->setCurrentIndex ( comboBox_text_index );
}

/************************************************************/
/*               GET_ADAK_STD_DOVIZLER_STRUCT               */
/************************************************************/

const char ** GET_ADAK_STD_DOVIZLER_STRUCT ()
{
    return (const char **)dovizler;
}
