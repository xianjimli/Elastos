
#ifndef __SKIAIOSTREAMADAPTOR_H__
#define __SKIAIOSTREAMADAPTOR_H__

#include "ext/frameworkext.h"
#include <SkStream.h>

using namespace Elastos;

CARAPI_(SkStream*) CreateSkiaInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize = 0);

CARAPI_(SkWStream*) CreateSkiaOutputStreamAdaptor(
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage);

#endif //__SKIAIOSTREAMADAPTOR_H__
