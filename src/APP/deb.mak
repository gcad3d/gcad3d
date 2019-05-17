# Debug 1=Development; Debug-ON
#       2=Distribute;  Debug-OFF
DEB = 1


ifeq "$(DEB)" "1"
  CPDEB = -DDEB -ggdb -pg
  LKDEB = -ggdb -pg
endif

# LKDEB = -rdynamic $(GUILK) -ggdb -pg ../libxa_test.so -Wl,-rpath .

#
# eof
