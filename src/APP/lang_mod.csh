#! /bin/csh

# change line starting with $1 into $2
# $1 - <keyword>
# S2 - text without "<keyword>=" enclosed in ''      (do not use | in text)
# example:
# ./lang_mod.csh Typ_modRepl 'replace obj; default = copy; Key PgUp/Dwn or "REPL"'

echo "modify ln " $1

if (`echo $* | wc -w` != "0") goto START
echo "change line starting with $1 "
goto ENDE


START:
# TESTBLOCK
#set fNam = msg_de.txt
# END TESTBLOCK
set keyWd = $1
shift
#echo $keyWd
#echo $*


foreach fn (`ls ../../doc/msg/* | grep msg_...txt`)
 cp -f $fn actMsgFil.txt
 #sed -n "s/^$keyWd.*/$keyWd=$*/" actMsgFil.txt > $fn
 cat actMsgFil.txt | sed "s|^$keyWd.*|$keyWd=$*|" > $fn
end

ENDE:
exit(0)
