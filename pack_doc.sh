#!/bin/sh

# create zipped doc-package ckitgui_doc_V<version>.zip

version=`cat src/gcad_version`

basDir=gcad3d


rm -f gcad3d_doc_V${version}.zip

zip gcad3d_doc_V${version}.zip -r doc/gui src

# -x "*.tag" "*.swp"


# eof
