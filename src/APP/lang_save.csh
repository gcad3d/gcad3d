#! /bin/csh

echo "save languag-files -> ./tmp/."

foreach fn (`ls -1 | grep msg_...txt`)
  echo $fn
  cp -f $fn ./tmp/.
end

ENDE:
exit(0)
