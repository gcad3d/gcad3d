#! /bin/csh

# ./lang_rest.csh

echo "copy back languag-files <- ../myTmp/."

foreach fn (`ls ../myTmp/* | grep msg_...txt`)
  echo $fn
  cp -f $fn ../../doc/msg/.
end

ENDE:
exit(0)
