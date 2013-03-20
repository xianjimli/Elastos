#==========================================================================
# Copyright (c) 2000-2011,  Elastos, Inc.  All Rights Reserved.
#==========================================================================
$(SPECIAL_TARGET) :
	if [ -a $(XDK_USER_LIB)/Elastos.Utility.lib ]; then \
	 $(MV) $(XDK_USER_LIB)/Elastos.Utility.lib .; \
	fi
	touch $(SPECIAL_TARGET)

