#!/bin/bash

. ../options.sh

# icons; if not set: /usr/share/gcad3d/
# gcad_dir_bas used for icons/
  export gcad_dir_bas=${gcad_dir_dev}

# executables (shared-libs ..)
# export gcad_dir_bin=${DirBasSer}/gCAD3D/binLinux${bitNr}/
# export gcad_dir_bin=${DirBasSer}/binLinux${bitNr}/
# export gcad_dir_bin=${gcad_dir_bin}
  export LD_LIBRARY_PATH=${gcad_dir_bin}

# def is $HOME; user-directory is <gcad_dir_local>/gCAD3D/ (cfg/..)
  #export gcad_dir_local=${DirBasSer}/
  export gcad_dir_local=${gcad_dir_dev}

# icons (def.: /usr/share/gcad3d/icons/)
  export gcad_dir_ico=${gcad_dir_dev}icons/

# html and msg-files
# def is /usr/share/gcad3d/doc/
  export gcad_dir_doc=${gcad_dir_dev}doc/



valgrind --leak-check=yes ${gcad_dir_bin}gCAD3D STP_U/4bar.gcad > t1 2>&1

# eof
