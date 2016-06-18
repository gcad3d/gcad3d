#!/bin/sh
# unset DIR_DEV
# unset DIR_BIN

. ../options.sh

echo `getconf LONG_BIT`"-bit-system"

echo "DIR_DEV =" ${DIR_DEV}
#echo "DIR_BIN =" ${DIR_BIN}


echo "gcad_dir_dev =" ${gcad_dir_dev}
echo "gcad_dir_bin =" ${gcad_dir_bin}

/bin/mkdir -p ${gcad_dir_bin}/plugins
/bin/mkdir -p ${gcad_dir_bin}/plugins/cut1

/bin/mkdir -p ${gcad_dir_dev}/src/tags



# eof
