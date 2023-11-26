# deb.mak  DEB ON or OFF
# MSYS2: do NOT enable profiling -pg - makes "32 bit pseudo relocation .. out of range"
# - and adds export sysmbol __fentry__   (PROBLEM ?)
# UIX:  no -fstack-check (-fstack-clash_protection used by gtk)


$(info - entering deb.mak $(OSTYP))

DEB := $(shell cat ./gcad_deb_$(OSTYP))

ifeq "$(DEB)" "ON"
# CPDEB = -DDEB -ggdb -pg -fstack-check -O0
  CPDEB := -DDEB -O0 -ggdb
# -Wformat-overflow -Wnonnull
  LKDEB = -ggdb
# -lmcheck does not work with gtk in overlay-lib
endif


$(info - DEB = $(DEB))
$(info - CPDEB = $(CPDEB))
$(info - LKDEB = $(LKDEB))
$(info - exit deb.mak)


# eof
