
#ifndef __CINPUTCHANNELHELPER_H__
#define __CINPUTCHANNELHELPER_H__

#include "_CInputChannelHelper.h"

CarClass(CInputChannelHelper)
{
public:
    /**
     * Creates a new input channel pair.  One channel should be provided to the input
     * dispatcher and the other to the application's input queue.
     * @param name The descriptive (non-unique) name of the channel pair.
     * @return A pair of input channels.  They are symmetric and indistinguishable.
     */
    CARAPI OpenInputChannelPair(
        /* [in] */ const String& name,
        /* [out] */ IInputChannel** inputChannel0,
        /* [out] */ IInputChannel** inputChannel1);
};


#endif //__CINPUTCHANNELHELPER_H__
