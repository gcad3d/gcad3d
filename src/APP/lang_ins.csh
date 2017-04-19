#! /bin/csh

# insert a line xy after line starting with $1
# example:
#./lang_ins.csh Project_obj__Prj_ Mirror_about_line Mirror: reflect objects

echo $1 $2

if (`echo $* | wc -w` != "0") goto START
echo "insert line after line starting with $1"
echo "1.Par: keyword of line after which to insert"
echo "following Parameters: line to insert"
goto ENDE

START:
set s1 = "$1"
shift

echo $* > actMsgLn.txt

echo " after " $s1 " insert:"
echo $*


# TEST ONLY:
# cp -f msg_de.txt actMsgFil.txt
# set fn = msg_de.txt


foreach fn (`ls -1 | grep msg_...txt`)
# echo change $fn
  cp -f $fn actMsgFil.txt
  sed "/^$s1/r actMsgLn.txt" actMsgFil.txt > $fn
end

ENDE:
exit(0)

