
#ifndef __HH_INPUTSTREAMADAPTOR_H
#define __HH_INPUTSTREAMADAPTOR_H

#include "ext/frameworkext.h"
#include <skia/core/SkStream.h>

using namespace Elastos;

ELAPI_(SkStream*) CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize);

#endif // __HH_INPUTSTREAMADAPTOR_H
