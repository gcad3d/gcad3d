#! /bin/bash
#set verbose

# find & touch all files with #include <$1>". cscope must be installed.
echo "fnd_inc |"$1"|"

# test if cscope is installed
if [ -z "`which cscope`" ]; then
  echo "****** ERROR - cscope is not installed ******"
  exit 1
fi

# test if includefilename given
if [ -z "$1" ]; then
  echo "****** ERROR - provide includefilename as parameter ******"
  exit 1
fi

# create list of gcad-sourcefiles in file srcFiles
make -f gcad3d.mak srclst > /dev/null

# add gui-sources
make -f gcad_gui__.mak srclst

# get cscope_tmp1 = list of files using the includefile
cscope -i srcFiles -L8 $1 > myCscope_tmp1

# get myCscope_tmp2 = script "touch <includefile>"
cat myCscope_tmp1 | cut -d' ' -f1 | uniq -u | sed "s/^/touch /" > myCscope_tmp2

# execute myCscope_tmp2
chmod +x myCscope_tmp2
./myCscope_tmp2

# display
cat myCscope_tmp2
echo "******* " `cat myCscope_tmp2 | wc -l` " files touched; continue with ./do c"
exit $?

# eof
