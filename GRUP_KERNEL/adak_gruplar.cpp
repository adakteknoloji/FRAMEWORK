#include <QList>
#include <QStringList>
#include <stdarg.h>
#include "adak_utils.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_gruplar.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   KAYDIN_GRUP_IDLERINI_BUL
***************************************************************************************/

QList<int> GRP_KAYDIN_GRUP_IDLERINI_BUL ( int program_id , int module_id ,int record_id )
{
    SQL_QUERY query    ( DB );

    query.PREPARE_SELECT ( "adak_kaydin_gruplari","gruplar","record_id = :record_id AND module_id = :module_id AND program_id = :program_id" ,"");

    query.SET_VALUE ( ":record_id" , record_id  );
    query.SET_VALUE ( ":module_id" , module_id  );
    query.SET_VALUE ( ":program_id", program_id );

    if ( query.SELECT() EQ 0 ) {
        return QList<int>();
    }

    query.NEXT();

    QStringList grup_idleri_str = query.VALUE(0).toString().split("\t");

    if (grup_idleri_str.size() EQ 0) {
        return QList<int>();
    }

    QList<int> grup_idleri;

    for (int i = 0 ; i < grup_idleri_str.size() ; i++ ) {
        grup_idleri << grup_idleri_str.at(i).toInt();
    }

    return grup_idleri;
}

/**************************************************************************************
                   KAYDIN_GRUPLARINI_BUL
***************************************************************************************/

QString GRP_KAYDIN_GRUPLARINI_BUL ( int program_id , int module_id ,int record_id ,
                                QList<int> * grup_idleri , QStringList * grup_adlari , QStringList * grup_kodlari)
{
    SQL_QUERY query    ( DB );

    query.PREPARE_SELECT ( "adak_kaydin_gruplari","gruplar","record_id = :record_id AND module_id = :module_id AND program_id = :program_id" ,"");

    query.SET_VALUE ( ":record_id" , record_id  );
    query.SET_VALUE ( ":module_id" , module_id  );
    query.SET_VALUE ( ":program_id", program_id );

    if ( query.SELECT() EQ 0 ) {
        return "";
    }

    query.NEXT();

    QStringList grup_idleri_str = query.VALUE(0).toString().split("\t",QString::SkipEmptyParts);

    if (grup_idleri_str.size() EQ 0) {
        return "";
    }

    for (int i = 0 ; i < grup_idleri_str.size() ; i++ ) {
        *grup_idleri << grup_idleri_str.at(i).toInt();
    }

    return GRP_GRUP_ADLARINI_BUL(grup_idleri , grup_adlari , grup_kodlari);
}

/**************************************************************************************
                   GRUP_ADLARINI_BUL
***************************************************************************************/

QString GRP_GRUP_ADLARINI_BUL ( QList<int> * grup_idleri , QStringList * grup_adlari , QStringList * grup_kodlari)
{
    SQL_QUERY query    ( DB );

    QString kaydin_gruplari;

    if (grup_idleri->size() EQ 0) {
        return "";
    }

    QString where_str = " ";

    for ( int i = 0 ; i < grup_idleri->size() ; i++ ) {
        if (i NE 0) {
            where_str.append(" OR");
        }
        where_str.append(QString(" grup_id=%1").arg(grup_idleri->at(i)));
    }

    query.PREPARE_SELECT("adak_gruplar","grup_adi,tam_grup_kodu",where_str,"");

    if ( query.SELECT() EQ 0 ) {
        return "";
    }

    while(query.NEXT() EQ true) {
        if (grup_kodlari NE NULL) {
            *grup_kodlari   << query.VALUE(1).toString();
	}
        *grup_adlari    << query.VALUE(0).toString();
        kaydin_gruplari.append(QString(query.VALUE(0).toString() + "\n"));
    }

    return kaydin_gruplari;
}

/**************************************************************************************
                   GRP_KAYDIN_GRUPLARINI_SIL
***************************************************************************************/

void GRP_KAYDIN_GRUPLARINI_SIL ( int program_id , int module_id ,int record_id )
{
    //Kaydi silmeden once tum gruplardan dusurulmesi lazim.
    GRP_KAYDI_GRUPLARA_EKLE(program_id,module_id,record_id,new QList<int>());

    SQL_QUERY query (DB);

    query.PREPARE_DELETE("adak_kaydin_gruplari", "record_id=:record_id AND program_id = :program_id AND module_id = :module_id");

    query.SET_VALUE(":record_id" ,  record_id   );
    query.SET_VALUE(":program_id",  program_id  );
    query.SET_VALUE(":module_id" ,  module_id   );

    query.DELETE();
}

/**************************************************************************************
                   GRP_KAYDI_GRUPLARA_EKLE
***************************************************************************************/

void GRP_KAYDI_GRUPLARA_EKLE ( int program_id , int module_id ,int record_id , QList<int> * grup_idleri )
{
    QString gruplar_str = "";

    SQL_QUERY query    ( DB );

    if (grup_idleri->size() NE 0) {
        for ( int i = 0 ; i < grup_idleri->size() ; i++ ) {
            gruplar_str.append("\t");
            gruplar_str.append(QVariant(grup_idleri->at(i)).toString());
            gruplar_str.append("\t");
        }
    }

    query.PREPARE_SELECT ( "adak_kaydin_gruplari","id,gruplar","record_id = :record_id AND module_id = :module_id AND program_id = :program_id" ,"");

    query.SET_VALUE ( ":record_id" , record_id  );
    query.SET_VALUE ( ":module_id" , module_id  );
    query.SET_VALUE ( ":program_id", program_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        int id              = query.VALUE(0).toInt();
        QStringList gruplar = query.VALUE(1).toString().split("\t",QString::SkipEmptyParts);

        query.PREPARE_UPDATE("adak_kaydin_gruplari","id","gruplar","id = :id");
        query.SET_VALUE(":gruplar" ,QVariant(gruplar_str).toByteArray());
        query.SET_VALUE(":id",id);
        query.UPDATE();

        for ( int i = 0 ; i < grup_idleri->size() ; i++ ) {
            if (gruplar.contains(QVariant(grup_idleri->at(i)).toString()) EQ true) {
                gruplar.removeAt(i);
                continue;
            }
            query.PREPARE_SELECT("adak_gruplar","gruptaki_kayit_sayisi","grup_id=:grup_id","");
            query.SET_VALUE(":grup_id",grup_idleri->at(i));
            if ( query.SELECT() EQ 0 ) {
                return;
            }
            query.NEXT();

            int value = query.VALUE(0).toInt();

            query.PREPARE_UPDATE("adak_gruplar","grup_id","gruptaki_kayit_sayisi","grup_id=:grup_id");
            query.SET_VALUE(":gruptaki_kayit_sayisi",(value+1));
            query.SET_VALUE(":grup_id",grup_idleri->at(i));
            query.UPDATE();
        }

        for ( int i = 0 ; i < gruplar.size() ; i++ ) {
            if (gruplar.at(i).toInt() NE 0 ) {
                query.PREPARE_SELECT("adak_gruplar","gruptaki_kayit_sayisi","grup_id=:grup_id","");
                query.SET_VALUE(":grup_id",gruplar.at(i));
                if ( query.SELECT() EQ 0 ) {
                    return;
                }
                query.NEXT();

                int value = query.VALUE(0).toInt();

                query.PREPARE_UPDATE("adak_gruplar","grup_id","gruptaki_kayit_sayisi","grup_id=:grup_id AND gruptaki_kayit_sayisi > 0 ");
                query.SET_VALUE(":gruptaki_kayit_sayisi",(value - 1));
                query.SET_VALUE(":grup_id",gruplar.at(i));
                query.UPDATE();
            }
        }

        return;
    }

    query.PREPARE_INSERT("adak_kaydin_gruplari","id", "record_id,gruplar,module_id,program_id ");

    query.SET_VALUE(":record_id", record_id    );
    query.SET_VALUE(":gruplar",   QVariant(gruplar_str).toByteArray() );
    query.SET_VALUE(":module_id", module_id    );
    query.SET_VALUE(":program_id",program_id   );

    query.INSERT();

    for ( int i = 0 ; i < grup_idleri->size() ; i++ ) {
        query.PREPARE_SELECT("adak_gruplar","gruptaki_kayit_sayisi","grup_id=:grup_id","");
        query.SET_VALUE(":grup_id",grup_idleri->at(i));
        if ( query.SELECT() EQ 0 ) {
            return;
        }
        query.NEXT();

        int value = query.VALUE(0).toInt();

        query.PREPARE_UPDATE("adak_gruplar","grup_id","gruptaki_kayit_sayisi","grup_id=:grup_id");
        query.SET_VALUE(":gruptaki_kayit_sayisi",(value+1));
        query.SET_VALUE(":grup_id",grup_idleri->at(i));
        query.UPDATE();;
    }
}

/**************************************************************************************
                   GRP_GRUBUN_ALT_GRUPLARINI_BUL
***************************************************************************************/

void GRP_GRUBUN_ALT_GRUPLARINI_BUL ( QList<int> gruplar, QList<int> * alt_grup_idleri )
{
    if (gruplar.isEmpty() EQ true) {
        return;
    }

    QString where_str = "";

    for ( int i = 0 ; i < gruplar.size() ; i++) {
        if ( i NE 0 ) {
            where_str.append("OR ");
        }
        where_str.append(QString("grup_id = %1 ").arg(gruplar.at(i)));
    }

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("adak_gruplar","grup_id , grup_adi , alt_grup_sayisi",where_str,"");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    QList<int> parent_gruplar;

    while ( query.NEXT() EQ true ) {
        if (query.VALUE(2).toInt() EQ 0) {
            if ( alt_grup_idleri->contains(query.VALUE(0).toInt()) EQ false ) {
                *alt_grup_idleri << query.VALUE(0).toInt();
            }
            continue;
        }
        parent_gruplar << query.VALUE(0).toInt();
    }

    if (parent_gruplar.isEmpty() EQ true) {
        return;
    }

    where_str = "";

    for ( int i = 0 ; i < parent_gruplar.size() ; i++) {
        if ( i NE 0 ) {
            where_str.append("OR ");
        }
        where_str.append(QString("grup_parent_id = %1 ").arg(parent_gruplar.at(i)));
    }

    query.PREPARE_SELECT("adak_gruplar","grup_id , grup_adi , alt_grup_sayisi",where_str,"");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    gruplar.clear();

    while ( query.NEXT() EQ true ) {
        gruplar << query.VALUE(0).toInt();
    }

    GRP_GRUBUN_ALT_GRUPLARINI_BUL(gruplar,alt_grup_idleri);
}

/**************************************************************************************
                   GRUBA_EKLI_KAYITLARI_BUL
***************************************************************************************/

QList<int> GRP_GRUBA_EKLI_KAYITLARI_BUL (int program_id, int module_id, QList<int> gruplar)
{
    if ( gruplar.size() EQ 0 ) {
        return QList<int>();
    }

    QList<int> alt_grup_idleri;

    GRP_GRUBUN_ALT_GRUPLARINI_BUL(gruplar,&alt_grup_idleri);

    gruplar = alt_grup_idleri;

    SQL_QUERY query    ( DB );
    QList<int>         record_ids;

    QString where_str = " (";

    for ( int i = 0 ; i < gruplar.size() ; i++ ) {
        if (i NE 0) {
            where_str.append(" OR");
        }

        //PostgreSQL de bytea de ilike olmadigindan cast etmemiz gerekli.
        if ( DB->GET_SQL_DRIVER_ENUM() EQ PSQL ) {
            where_str.append(QString(" convert_from(gruplar , 'UTF8') like '%\011%1\011%'").arg(gruplar.at(i)));
        }
        else if (DB->GET_SQL_DRIVER_ENUM() EQ ORACLE ) {
            //Oracle da ise db fonksiyonlari kullanilmali.
            where_str.append(QString("(dbms_lob.instr(gruplar, utl_raw.CAST_TO_RAW('%\t%1\t%')) > 0)").arg(gruplar.at(i)));
        }
        else {
            where_str.append(QString(" gruplar like '%\t%1\t%'").arg(gruplar.at(i)));
        }
    }
    where_str.append(QString(") AND (program_id=%1 AND module_id = %2)").arg(QVariant(program_id).toString())
                                                                        .arg(QVariant(module_id ).toString()));

    query.PREPARE_SELECT("adak_kaydin_gruplari","record_id",where_str,"");

    if ( query.SELECT() EQ 0 ) {
        return QList<int>();
    }

    while(query.NEXT() EQ true) {
        record_ids << query.VALUE(0).toInt();
    }

    return record_ids;
}

/**************************************************************************************
                   GRP_GRUP_EKLE
***************************************************************************************/

int GRP_GRUP_EKLE ( ADAK_GRUP_STRUCT * grup )
{
    int     grup_seviyesi       = 0;
    int     alt_grup_sayisi     = 0;
    QString tam_grup_kodu       = grup->grup_kodu;

    SQL_QUERY query ( DB );

    if (grup->grup_parent_id > 0) {
        query.PREPARE_SELECT("adak_gruplar","grup_seviyesi, tam_grup_kodu,alt_grup_sayisi","grup_id = :grup_id","");
        query.SET_VALUE(":grup_id" , grup->grup_parent_id);

        if (query.SELECT() NE 0) {
            query.NEXT();

            grup_seviyesi          = query.VALUE("grup_seviyesi").toInt() + 1;
            tam_grup_kodu.prepend(QString (" " + query.VALUE("tam_grup_kodu").toString()));
            alt_grup_sayisi        = query.VALUE("alt_grup_sayisi").toInt();
        }

        query.PREPARE_UPDATE("adak_gruplar","grup_id","grup_son_seviye_mi, alt_grup_sayisi = alt_grup_sayisi + 1","grup_id = :grup_id");
        query.SET_VALUE(":grup_son_seviye_mi",0);
        query.SET_VALUE(":alt_grup_sayisi"   ,(alt_grup_sayisi+1));
        query.SET_VALUE(":grup_id"           ,grup->grup_parent_id);
        query.UPDATE();
    }

    query.PREPARE_INSERT("adak_gruplar","grup_id","grup_parent_id,grup_seviyesi, "
                         "grup_son_seviye_mi, gruptaki_kayit_sayisi,alt_grup_sayisi,modul_id, "
                         "program_id, grup_kodu,grup_adi,tam_grup_kodu");

    query.SET_VALUE(":grup_parent_id"       , grup->grup_parent_id);
    query.SET_VALUE(":grup_seviyesi"        , grup_seviyesi);
    query.SET_VALUE(":grup_son_seviye_mi"   , 1);
    query.SET_VALUE(":gruptaki_kayit_sayisi", 0);
    query.SET_VALUE(":alt_grup_sayisi"      , 0);
    query.SET_VALUE(":modul_id"             , grup->modul_id);
    query.SET_VALUE(":program_id"           , grup->program_id);
    query.SET_VALUE(":grup_kodu"            , grup->grup_kodu);
    query.SET_VALUE(":grup_adi"             , grup->grup_adi);
    query.SET_VALUE(":tam_grup_kodu"        , tam_grup_kodu);

    return query.INSERT();
}

/**************************************************************************************
                   GRP_GRUP_EKLENDIMI
***************************************************************************************/

bool GRP_GRUP_EKLENDIMI(int program_id, int module_id)
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("adak_gruplar","grup_id","program_id = :program_id AND modul_id = :module_id ","");

    query.SET_VALUE(":program_id" , program_id);
    query.SET_VALUE(":module_id"  , module_id );

    if (query.SELECT() EQ 0) {
        return false;
    }

    return true;
}




