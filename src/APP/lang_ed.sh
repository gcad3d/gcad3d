#!/bin/sh

# get bitNr DIR_DEV gcad_dir_dev gcad_dir_bin
. ../options.sh

gcad_dir_doc=${gcad_dir_dev}'doc/'
echo "gcad_dir_doc = " ${gcad_dir_doc}

cmdVi=${DIR_BIN}'vi.sh'

cd ${gcad_dir_doc}msg

for fn in `ls -1 | grep msg_...txt`; do
  $cmdVi $fn
done

exit
