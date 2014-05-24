Veritabanı ismini düzeltmek için

sqlite3 adak_yonetim.db                 
sqlite> select * from ynt_veritabanlari;
1|xyz|XYZ Veritabanı|
sqlite> update ynt_veritabanlari SET veritabani_ismi="def" where veritabani_id=1;
sqlite> update ynt_veritabanlari SET veritabani_tanimi="DEF Veritabanı" where veritabani_id=1;


Son olarak 

mv adak_xyz.db adak_def.db


