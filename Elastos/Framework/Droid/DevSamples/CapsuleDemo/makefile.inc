#==========================================================================
# Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
#==========================================================================
#cpfile: $(TARGET)
$(SPECIAL_TARGET):
##########################################################
# copy capsule and application files
##########################################################

	cp -f -r $(MAKEDIR)/res $(XDK_TARGETS)/CapsuleDemo 2>/dev/null
	rm -f -r `find $(XDK_TARGETS) -name *.svn` 2>/dev/null
	rm -f -r `find $(XDK_TARGETS) -name *.bak` 2>/dev/null
	@touch $@
