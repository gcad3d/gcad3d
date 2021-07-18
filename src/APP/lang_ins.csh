#!/bin/csh

# enable using '*' in parameters ..
set noglob

if (`echo $* | wc -w` != "0") goto START
echo "insert line after line starting with <parameter-1>"
echo "1.Par: keyword of line after which to insert"
echo "following Parameters: line to insert"
echo "See INF_MSG_new"
goto ENDE

START:
echo "start.."
set s1 = "$1"
shift

echo $* > actMsgLn.txt

echo " after " $s1 " insert:"
echo $*


# TEST ONLY:
# cp -f msg_de.txt actMsgFil.txt
# set fn = msg_de.txt

unset noglob

foreach fn (`ls ../../doc/msg/* | grep msg_...txt`)
 echo change $fn
  cp -f $fn actMsgFil.txt
  sed "/^$s1/r actMsgLn.txt" actMsgFil.txt > $fn
end


ENDE:
exit(0)

