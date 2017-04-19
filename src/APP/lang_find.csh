#! /bin/csh

# example: 

echo "find ln " $1

if (`echo $* | wc -w` != "0") goto START
echo "find lines starting with $1"
goto ENDE


START:
# TEST ONLY:
#set fNam = msg_de.txt


foreach fn (`ls -1 | grep msg_...txt`)
  echo "-------- File" $fn
  grep $1 $fn
end

ENDE:
exit(0)
