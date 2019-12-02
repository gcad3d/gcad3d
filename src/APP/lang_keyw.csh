#! /bin/csh
#
# change keyword in all files ../../doc/msg/msg*.txt
#
if ($#argv < 2) then 
  echo " give keyWd-old  keyWd-new"
  exit(0)
endif
#
echo "change keywd |"$1"| -> |"$2"|"
cd ../../doc/msg/&&${DIR_UTI}changeall $1 $2 msg*.txt
#
