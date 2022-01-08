#!/bin/sh
#
# get (return) the install-type; "rpm" or "deb"
# check for rpm or dpkg[-query]


if [ ! -z "`which dnf`" ] ; then
  echo "rpm"
else
  echo "deb"
fi


# EOF
