#!/bin/sh
#
# compile files using a certain function.
# in cscope get filList (Find egrep "ut_mem.h") 
# in cscope pipe filListe into file t1 (do '> t1')
# then ./do fDep (starts this func)
# Example: compile all files with '#include "../xa/xa.h"  .. '
#   ./ed (cscope) "Find this egrep pattern:"  "../xa/xa.h"  > t1
#   ./do fDep
#
# create fDep_tmp.sh = script "touch <files>"
cut -d' ' -f1 t1 | uniq -u | sed "s/^/touch /" > fDep_tmp.sh
# execute fDep.txt
chmod +x fDep_tmp.sh
./fDep_tmp.sh
rm -rf fDep_tmp.sh
#
#eof
