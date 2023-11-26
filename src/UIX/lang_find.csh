#! /bin/csh

# example: 
# ./lang_find.csh Typ_modRepl

echo "find ln " $1

if (`echo $* | wc -w` != "0") goto START
echo "find lines starting with <parameter-1>"
goto ENDE


START:
# TEST ONLY:
#set fNam = msg_de.txt


foreach fn (`ls ../../doc/msg/* | grep msg_...txt`)
  echo "-------- File" $fn
  grep $1 $fn
end

ENDE:
exit(0)
