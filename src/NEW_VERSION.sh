#!/bin/sh

# create new version-info
vi gcad_version


# create new version-date
vi gcad_date

gcad_version=`cat gcad_version`
gcad_date=`cat gcad_date`

echo "#define INIT_TXT \" gCAD3D Version ${gcad_version} / ${gcad_date}\"" > xa/gcad_version.h


# EOF

