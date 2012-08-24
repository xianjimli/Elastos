
#ifndef __HH_CREATEOUTPUTSTREAMADAPTOR_H
#define __HH_CREATEOUTPUTSTREAMADAPTOR_H


#include "ext/frameworkext.h"
#include "skia/core/SkStream.h"

SkStream* CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize = 0);

SkWStream* CreateOutputStreamAdaptor(
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage);

#endif // __HH_CREATEOUTPUTSTREAMADAPTOR_H

