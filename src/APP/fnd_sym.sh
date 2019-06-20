#! /bin/bash
#set verbose

# find & touch all files using symbol <$1>
echo "fnd_sym |"$1"|"

# test if cscope is installed
if [ -z "`which cscope`" ]; then
  echo "****** ERROR - cscope is not installed ******"
  exit 1
fi

# test if symbolname given
if [ -z "$1" ]; then
  echo "****** ERROR - provide symbolname as parameter ******"
  exit 1
fi

# create list of gcad-sourcefiles in file srcFiles
make -f gcad3d.mak srclst > /dev/null

# add gui-sources
make -f gcad_gui__.mak add_srclst

# get cscope_tmp1 = list of files using the symbol $1
cscope -i srcFiles -L0 $1 > myCscope_tmp1

# get myCscope_tmp2 = script "touch <includefile>"
cat myCscope_tmp1 | cut -d' ' -f1 | uniq | sed "s/^/touch /" > myCscope_tmp2

# execute myCscope_tmp2
chmod +x myCscope_tmp2
./myCscope_tmp2

# display
cat myCscope_tmp2
echo "******* " `cat myCscope_tmp2 | wc -l` " files touched; continue with ./do c"
exit $?

#eof
