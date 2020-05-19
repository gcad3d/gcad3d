# Debug 1=Development; Debug-ON
#       2=Distribute;  Debug-OFF
# -pg does produce a gmon.out (add for CPDEB and LKDEB)
# -ggdb or -g ?
DEB = 1


ifeq "$(DEB)" "1"
  CPDEB = -DDEB -ggdb -fstack-check -O0
# -Wformat-overflow -Wnonnull
  LKDEB = -ggdb
# -lmcheck does not work with gtk in overlay-lib
endif

# LKDEB = -rdynamic $(GUILK) -ggdb -pg ../libxa_test.so -Wl,-rpath .

#
# eof
