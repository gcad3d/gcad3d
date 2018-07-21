#! /bin/csh

# delete line starting with $1
# example: 

echo "del. ln " $1

if (`echo $* | wc -w` != "0") goto START
echo "delete line starting with $1"
goto ENDE


START:
# TEST ONLY:
#set fNam = msg_de.txt


foreach fn (`ls ../../doc/msg/* | grep msg_...txt`)
 cp -f $fn actMsgFil.txt
 sed -n "/^$1/! p" actMsgFil.txt > $fn
end

ENDE:
exit(0)
