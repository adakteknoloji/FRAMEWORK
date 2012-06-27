#!/bin/bash

#mysqldump  --default-character-set=utf8 --compatible=ansi --skip-extended-insert --compact -h 10.1.0.1 -P 3308 -u root -p adak_yonetim  > dumpfile

if [ "x$1" == "x" ]; then
	  echo "Usage: $0 <dumpname>"
	    exit
    fi

rm -f $1.sql
rm -f $1.err
rm -f $1.db

    cat $1 |
    grep -v ' KEY ' |
    grep -v ' UNIQUE KEY ' |
    grep -v ' PRIMARY KEY ' |
    grep -v '40101 ' |
    sed '/^SET/d' |
    sed 's/ unsigned / /g' |
    sed 's/ COLLATE /collate /g' |
    sed 's/ collate utf8_turkish_ci//g' |
    sed 's/ auto_increment/ autoincrement/g' |
    sed 's/ AUTO_INCREMENT/ autoincrement/g' |
    sed 's/ autoincrement/ primary key autoincrement/g' |
    sed 's/ NOT NULL primary key / primary key /g' |
    sed 's/ smallint([0-9]*) / integer /g' |
    sed 's/ tinyint([0-9]*) / integer /g' |
    sed 's/ int([0-9]*) / integer /g' |
    sed 's/ character set [^ ]* / /g' |
    sed 's/ enum([^)]*) / varchar(255) /g' |
    sed 's/ on update [^,]*//g' |
    sed 's/\\r\\n/\\n/g' |
    sed 's/\\"/"/g' |
    perl -e 'local $/;$_=<>;s/,\n\)/\n\)/gs;print "begin;\n";print;print "commit;\n"' |
    perl -pe '
    if (/^(INSERT.+?)\(/) {
	      $a=$1;
	        s/\\'\''/'\'\''/g;
		  s/\\n/\n/g;
		    s/\),\(/\);\n$a\(/g;
	    }
	    ' > $1.sql
	    cat $1.sql | sqlite3 $1.db > $1.err
	    ERRORS=`cat $1.err | wc -l`
	    if [ $ERRORS == 0 ]; then
		      echo "Conversion completed without error. Output file: $1.db"
		        #rm $1.sql
			  #rm $1.err
		  else
			    echo "There were errors during conversion.  Please review $1.err and $1.sql for details."
		    fi
