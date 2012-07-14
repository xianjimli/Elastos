#==========================================================================
# Copyright (c) 2000-2011,  Elastos, Inc.  All Rights Reserved.
#==========================================================================

cpfile:
	@echo Making directory %XDK_TARGETS%/fbreader data...
	if [ ! -d $(XDK_TARGETS)/fbdata ]; then \
	$(MKDIR) $(XDK_TARGETS)/fbdata; \
	fi

	if [ ! -d $(XDK_TARGETS)/fbdata/default ]; then \
	$(MKDIR) $(XDK_TARGETS)/fbdata/default; \
	fi

	if [ ! -d $(XDK_TARGETS)/fbdata/encodings ]; then \
	$(MKDIR) $(XDK_TARGETS)/fbdata/encodings; \
	fi

	if [ ! -d $(XDK_TARGETS)/fbdata/resources ]; then \
	$(MKDIR) $(XDK_TARGETS)/fbdata/resources; \
	fi

	@echo Copying files to %XDK_TARGETS% ...
	$(CP) $(MAKEDIR)/default/*.xml $(XDK_TARGETS)/fbdata/default
	$(CP) $(MAKEDIR)/encodings/* $(XDK_TARGETS)/fbdata/encodings
	$(CP) $(MAKEDIR)/resources/*.xml $(XDK_TARGETS)/fbdata/resources
	$(CP) $(MAKEDIR)/languagePatterns.zip $(XDK_TARGETS)/fbdata/
	$(CP) $(MAKEDIR)/unicode.xml.gz $(XDK_TARGETS)/fbdata/
#	$(RMDIR) `find $(XDK_TARGETS)/fbdata/encodings -name *.svn` 2>/dev/null
