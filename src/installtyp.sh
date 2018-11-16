#!/bin/sh
#
# get (return) the install-type; "rpm" or "deb"


if [ ! -z "`which rpm`" ] ; then
  echo "rpm"
else
  echo "deb"
fi


# EOF
