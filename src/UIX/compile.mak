# compile nn.c -> nn.o

#$(compile.mak)/%.o:

.c.o:
	@echo ====================== compile $@ ========================
	@echo "VPATH="$(VPATH)
	$(CC) -c $(CPFLG) $<
	#	if test $(@D) != "../xa"; then mv $(@F) $@; fi
	mv -f $(@F) "$(gcad_dir_bin)."
	#	etags -f $(*F).tag $<
	rm -f ../tags/$(*F).tag
	ctags -f ../tags/$(*F).tag -IMemTab --c-kinds=dfstvx --tag-relative=yes --excmd=pattern $<


.cpp.o:
	@echo ================= Cpp $@ ========================
	# CPPFLG not yet set
	$(CC) -c $(CPPFLG) $<
	mv -f $(@F) "$(gcad_dir_bin)."
	rm -f ../tags/$(*F).tag
	ctags -f ../tags/$(*F).tag -IMemTab --tag-relative=yes $<

# EOF
